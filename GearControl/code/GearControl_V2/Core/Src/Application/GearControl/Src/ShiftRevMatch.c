/*
 * ShiftRevMatch.c
 *
 *  Created on: 08.08.2022
 *      Author: Patryk Wittbrodt
 */

#include "RPMTables.h"
#include "GearControl.h"
#include "DefineConfig.h"
#if CONFIG_ENABLE_REV_MATCH
#include "ShiftRevMatch.h"
#include "ClutchControl.h"
#include "GearWatchdog.h"
#include "DBW.h"
#include "CAN.h"
#include "Utils.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

#define RPM_THROTTLE_LUT_CNT (16U)

//! Engine RPM Lookup Table X values
static const float X_Rpm[RPM_THROTTLE_LUT_CNT] = {
	812.5f,
	1625.0f,
	2437.5f,
	3250.0f,
	4062.5f,
	4875.0f,
	5687.5f,
	6500.0f,
	7312.5f,
	8215.0f,
	8937.5f,
	9750.0f,
	10562.5f,
	11375.0f,
	12187.5f,
	13000.0f
};

//! Throttle Degrees Lookup Table Y values
static const float Y_ThrottleDeg[RPM_THROTTLE_LUT_CNT] = {
	62.5f,
	125.0f,
	187.5f,
	250.0f,
	312.5f,
	375.0f,
	437.5f,
	500.0f,
	562.5f,
	625.0f,
	687.5f,
	750.0f,
	812.5f,
	875.0f,
	937.5f,
	1000.0f
};

//! throttle degrees <- RPM map
static const table_1d RpmThrottleMap = {
	.x_length = RPM_THROTTLE_LUT_CNT,
	.x_values = X_Rpm,
	.y_values = Y_ThrottleDeg
};

#if REVMATCH_FROM_ENGINE_RPM
#define GEAR_1_TRANSMISSION_RATIO (2.3125f)
#define GEAR_2_TRANSMISSION_RATIO (1.857f)
#define GEAR_3_TRANSMISSION_RATIO (1.565f)
#define GEAR_4_TRANSMISSION_RATIO (1.35f)
#define GEAR_5_TRANSMISSION_RATIO (1.238f)
#define GEAR_6_TRANSMISSION_RATIO (1.136f)

#define DOWNSHIFT_RPM_G1_MULTIPLIER (GEAR_1_TRANSMISSION_RATIO / GEAR_2_TRANSMISSION_RATIO) //!< 124.53%
#define DOWNSHIFT_RPM_G2_MULTIPLIER (GEAR_2_TRANSMISSION_RATIO / GEAR_3_TRANSMISSION_RATIO) //!< 118.69%
#define DOWNSHIFT_RPM_G3_MULTIPLIER (GEAR_3_TRANSMISSION_RATIO / GEAR_4_TRANSMISSION_RATIO) //!< 115.93%
#define DOWNSHIFT_RPM_G4_MULTIPLIER (GEAR_4_TRANSMISSION_RATIO / GEAR_5_TRANSMISSION_RATIO) //!< 109.05%
#define DOWNSHIFT_RPM_G5_MULTIPLIER (GEAR_5_TRANSMISSION_RATIO / GEAR_6_TRANSMISSION_RATIO) //!< 108.98%

//! RPM multiplier table for available gears
static const float downshiftRpmMultiplier[GEAR_6] = {
	[GEAR_1] = DOWNSHIFT_RPM_G1_MULTIPLIER,
	[GEAR_N] = 0.0f,
	[GEAR_2] = DOWNSHIFT_RPM_G2_MULTIPLIER,
	[GEAR_3] = DOWNSHIFT_RPM_G3_MULTIPLIER,
	[GEAR_4] = DOWNSHIFT_RPM_G4_MULTIPLIER,
	[GEAR_5] = DOWNSHIFT_RPM_G5_MULTIPLIER
};

#endif // REVMATCH_FROM_WHEEL_RPM

#if REVMATCH_FROM_WHEEL_RPM
static const table_1d *const WheelRpmGear_Map[GEAR_6 + 1U] = {
	[GEAR_1] = &G1_rpmMap,
	[GEAR_N] = NULL,
	[GEAR_2] = &G2_rpmMap,
	[GEAR_3] = &G3_rpmMap,
	[GEAR_4] = &G4_rpmMap,
	[GEAR_5] = &G5_rpmMap,
	[GEAR_6] = &G6_rpmMap
};
#endif // REVMATCH_FROM_WHEEL_RPM

typedef void (*GetCANDataFuncPtr)(void);
typedef RevMatchStates (*ThrottleBlipFuncPtr)(void);

//!< Shift Rev Match handler struct
typedef struct {
	RevMatchStates state;
	/* Data associate variables */
	bool_t rpmDataNew;
	bool_t wheelSpeedDataNew;
	uint32_t rpmData;
	uint32_t wheelSpeedData;
	float targetThrottle;
	GearStates gear;
	/* Watchdog */
	GearWatchdogType *const watchdog;
	GetCANDataFuncPtr getCanDataFunc;
	ThrottleBlipFuncPtr throttleBlipFunc;
} ShiftRevMatchHandler;

//! This must be before injectorsCutWdg declaration
static void ShiftRevMatchWatchdogElapsedTrigger(void);

//! This must be before shiftRevMatch_ declaration
#if REVMATCH_FROM_ENGINE_RPM
static inline void ShiftRevMatch_CAN_GetEngineRPM(void);
static inline RevMatchStates ShiftRevMatch_ThrottleBlipEngineRPM(void);
#elif REVMATCH_FROM_WHEEL_RPM
static inline void ShiftRevMatch_CAN_GetWheelRPM(void);
static inline RevMatchStates ShiftRevMatch_ThrottleBlipWheelRPM(void);
#endif // REVMATCH_FROM_ENGINE_RPM

//! Injectors Cut watchdog
static GearWatchdogType revMatchWdg = {
		.elapsedTrigger = ShiftRevMatchWatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = SHIFT_REVMATCH_WATCHDOG_TIMEOUT_MS
};

//<! Shift Rev Match handler
static ShiftRevMatchHandler shiftRevMatch_ = {
	.state = REVMATCH_INACTIVE,
	/* Data associate variables */
	.rpmDataNew = FALSE,
	.wheelSpeedDataNew = FALSE,
	.rpmData = 0U,
	.wheelSpeedData = 0U,
	.targetThrottle = 0.0f,
	.gear = GEAR_UNKNOWN,
	/* Watchdog */
	.watchdog = &revMatchWdg,
#if REVMATCH_FROM_ENGINE_RPM
	.getCanDataFunc = ShiftRevMatch_CAN_GetEngineRPM,
	.throttleBlipFunc = ShiftRevMatch_ThrottleBlipEngineRPM
#elif REVMATCH_FROM_WHEEL_RPM
	.getCanDataFunc = ShiftRevMatch_CAN_GetWheelRPM,
	.throttleBlipFunc = ShiftRevMatch_ThrottleBlipWheelRPM
#endif
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline bool_t ShiftRevMatch_TargetReached(void);
static void ShiftRevMatch_StateMachine(void);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */


/**
 * @brief Watchdog elapsed function.
 * 
 * If the rev match is active, and the rev match watchdog has elapsed,
 * then set the rev match state to finished.
 */
static void ShiftRevMatchWatchdogElapsedTrigger(void)
{
	shiftRevMatch_.state = REVMATCH_FINISHED;
}

/**
 * @brief Check if target throttle position has been reached.
 *
 * @return TRUE if the absolute error between the target position and the current position is less than
 * the acceptable position error, FALSE otherwise.
 */
static inline bool_t ShiftRevMatch_TargetReached(void)
{
	return (DBW_GetTargetPositionAbsError() <= SHIFT_REVMATCH_ACCEPTABLE_POSITION_ERROR);
}

#if REVMATCH_FROM_ENGINE_RPM
/**
 * @brief Get Engine RPM data from CAN Bus.
 * 
 * Read CAN data engine RPM's only when buffer is valid and clutch is not engaged.
 */
static inline void ShiftRevMatch_CAN_GetEngineRPM(void)
{
	uint8_t *const emuBlackMsgBuff = CAN_GetRxNewData(CAN_RX_MSG_EMU_BLACK);

	/* Read CAN data engine RPM's only when buffer is valid and clutch is not engaged */
	if ((emuBlackMsgBuff != NULL) &&
		(!ClutchControl_IsEngaged()) &&
		(shiftRevMatch_.state != REVMATCH_THROTTLE_BLIP)) {
		/* Get engine RPM from EMU Black */
		const uint32_t engineRPM = (uint32_t)emuBlackMsgBuff[CAN_DATA_BYTE_0];

		if ((engineRPM >= ENGINE_RPM_MIN) && (engineRPM <= ENGINE_RPM_MAX)) {
			shiftRevMatch_.rpmData = engineRPM;
			shiftRevMatch_.rpmDataNew = TRUE;
		}
	}
}

/**
 * @brief Throttle Blip handler.
 * 
 * If the gear is not neutral, and the RPM data is new, then calculate the target RPM for the lower
 * gear, translate that to a target throttle position, and set the DBW control to that throttle
 * position.
 * 
 * @return The next state of the state machine.
 */
static inline RevMatchStates ShiftRevMatch_ThrottleBlipEngineRPM(void)
{
	RevMatchStates nextState = REVMATCH_THROTTLE_BLIP;

	if (shiftRevMatch_.gear < GEAR_6) {
		if (shiftRevMatch_.rpmDataNew) {
			/* Get multiplier value from look-up table RPM Multiplier <- Gear */
			const float rpmMultiplier = downshiftRpmMultiplier[shiftRevMatch_.gear];

			/* Neutral will result in 0.0, rest of gears are valid */
			if (rpmMultiplier > 0.0f) {
				/* Calculate target RPM for lower gear */
				const float targetRPM = shiftRevMatch_.rpmData * rpmMultiplier;
				/* Translate target throttle position <- target RPM from interpolation */
				shiftRevMatch_.targetThrottle = Utils_interpolateTable1d(&RpmThrottleMap, targetRPM);

				/* Try to set Drive-By-Wire control from rev match */
				if (DBW_RevMatchSetControl(&shiftRevMatch_.targetThrottle) == REV_MATCH_DBW_OK) {
					/* DBW Control from rev match successful, start watchdog */
					GearWatchdog_Start(shiftRevMatch_.watchdog);
					/* Poll throttle target */
					nextState = REVMATCH_POLL_TARGET;
					/* Reset RPM data flag */
					shiftRevMatch_.rpmDataNew = FALSE;
				}
			}
		} else {
			nextState = REVMATCH_INACTIVE;
		}
	} else {
		nextState = REVMATCH_INACTIVE;
	}

	return nextState;
}

#elif REVMATCH_FROM_WHEEL_RPM
static inline void ShiftRevMatch_CAN_GetWheelRPM(void)
{
	uint8_t *const telemetryMsgBuff = CAN_GetRxNewData(CAN_RX_MSG_TELEMETRY);

	if ((telemetryMsgBuff != NULL) && (!shiftRevMatch_.throttleTriggered)) {
		/* Get wheel RPM from telemetry */
		const float wheelRPM = (float)telemetryMsgBuff[CAN_DATA_BYTE_0];

		if ((wheelRPM >= WHEEL_RPM_MIN) && (wheelRPM <= WHEEL_RPM_MAX)) {
			shiftRevMatch_.wheelSpeedData = wheelRPM;
			shiftRevMatch_.wheelSpeedDataNew = TRUE;
		}
	}
}

static void ShiftRevMatch_ThrottleBlipWheelRPM(void)
{
	/* TODO when telemetry stm works */
}
#endif // REVMATCH_FROM_ENGINE_RPM

/**
 * @brief Shift Rev Match main state machine.
 */
static void ShiftRevMatch_StateMachine(void)
{
	/* Get RPM data from CAN Bus */
	shiftRevMatch_.getCanDataFunc();

	switch (shiftRevMatch_.state) {
		case REVMATCH_THROTTLE_BLIP:
			shiftRevMatch_.state = shiftRevMatch_.throttleBlipFunc();
			break;

		case REVMATCH_POLL_TARGET:
			/* Poll throttle target position until it's reached */
			if (ShiftRevMatch_TargetReached()) {
				shiftRevMatch_.state = REVMATCH_FINISHED;
			}
			break;

		case REVMATCH_FINISHED:
			/* Rev match finished - watchdog elapsed or throttle target reached */
			ShiftRevMatch_Finish();
			break;

		case REVMATCH_INACTIVE:
		default:
			/* Ensure that DBW works in normal operation */
			DBW_RevMatchRestoreNormalOperation();
			break;
	}
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

/**
 * @brief Initialization of the Shift Rev Match module.
 * 
 * @return an error code.
 */
ErrorEnum ShiftRevMatch_Init(void)
{
	ErrorEnum err = ERROR_OK;

	/* Initialize Rev match Watchdog */
	err = GearWatchdog_Init(&revMatchWdg);

	return err;
}

/**
 * @brief Check if Rev Match is finished.
 * 
 * @return TRUE if rev match is finished, FALSE otherwise.
 */
bool_t ShiftRevMatch_IsFinished(void)
{
	return ((shiftRevMatch_.state == REVMATCH_FINISHED) ||
			(shiftRevMatch_.state == REVMATCH_INACTIVE));
}

/**
 * @brief Trigger rev match sequence.
 * 
 * Set downshift gear and state to throttle blip to start the sequence.
 * 
 * @param revMatchGear The gear that is being downshifted to.
 */
void ShiftRevMatch_Trigger(const GearStates revMatchGear)
{
	shiftRevMatch_.gear = GEAR_UNKNOWN;

	if (revMatchGear < GEAR_6) {
		shiftRevMatch_.gear = revMatchGear;
		shiftRevMatch_.state = REVMATCH_THROTTLE_BLIP;
	}
}

/**
 * @brief Rev Match finished state handler.
 * 
 * Restore DBW to normal operation, feed watchdog and reset variables.
 */
void ShiftRevMatch_Finish(void)
{
	/* Restore normal DBW operation - from throttle pedal position */
	DBW_RevMatchRestoreNormalOperation();

	/* Feed Watchdog */
	GearWatchdog_Feed(shiftRevMatch_.watchdog);

	/* Reset variables */
	shiftRevMatch_.gear = GEAR_UNKNOWN;
}

/**
 * @brief Main process function that is called from the Scheduler.
 */
void ShiftRevMatch_Process(void)
{
	ShiftRevMatch_StateMachine();
}
#else
void ShiftRevMatch_Trigger(const GearStates revMatchGear) { (void)revMatchGear; }
void ShiftRevMatch_Process(void) {}
ErrorEnum ShiftRevMatch_Init(void) { return ERROR_OK; }
bool_t ShiftRevMatch_IsFinished(void) {return FALSE; }
#endif // CONFIG_ENABLE_REV_MATCH

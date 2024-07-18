/*
 * ShiftRevMatch.c
 *
 *  Created on: 08.08.2022
 *      Author: Patryk Wittbrodt
 */

#include "RPMTables.h"
#include "GearControl.h"
#include "DefineConfig.h"
#if CONFIG_ENABLE_THROTTLE_BLIP
#include "ShiftRevMatch.h"
#include "ClutchControl.h"
#include "GearWatchdog.h"
#include "DBW.h"
#include "CAN.h"
#include "Utils.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

static const table_1d RpmThrottleMap[GEAR_6 + 1U] = {
	[GEAR_1] = {
		.x_length = 0,
	},
	[GEAR_N] = {
		.x_length = 0,
	},
	[GEAR_2] = {
		.x_length = RPM_THROTTLE_LUT_CNT,
		.x_values = X_Rpm,
		.y_values = Y_Gear2ThrottleMap
	},
	[GEAR_3] = {
		.x_length = RPM_THROTTLE_LUT_CNT,
		.x_values = X_Rpm,
		.y_values = Y_Gear3ThrottleMap
	},
	[GEAR_4] = {
		.x_length = RPM_THROTTLE_LUT_CNT,
		.x_values = X_Rpm,
		.y_values = Y_Gear4ThrottleMap
	},
	[GEAR_5] = {
		.x_length = RPM_THROTTLE_LUT_CNT,
		.x_values = X_Rpm,
		.y_values = Y_Gear5ThrottleMap
	},
	[GEAR_6] = {
		.x_length = 0,
	},
};

#if REVMATCH_FROM_ENGINE_RPM

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
	bool_t rpmNew;
	bool_t wheelSpeedDataNew;
	uint16_t rpm;
	uint16_t wheelSpeedData;
	float targetThrottle;
	GearStates gear;
	/* Watchdog */
	GearWatchdogType *const watchdog;
	GetCANDataFuncPtr getCanDataFunc;
	ThrottleBlipFuncPtr throttleBlipFunc;
	SwTimerType freshTimer;
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
	.state = REVMATCH_IDLE,
	/* Data associate variables */
	.rpmNew = FALSE,
	.wheelSpeedDataNew = FALSE,
	.rpm = 0U,
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
	uint8_t *const canBuff = CAN_GetRxNewData(CAN_RX_MSG_EMU_BLACK);

	/* Read CAN data engine RPM's only when buffer is valid and clutch is not engaged */
	if ((canBuff != NULL) &&
		(shiftRevMatch_.state == REVMATCH_IDLE)) {
		/* Get engine RPM from EMU Black */
		const uint16_t engineRPM = (uint16_t)(canBuff[CAN_DATA_BYTE_0] << 8U |
											  canBuff[CAN_DATA_BYTE_1]);

		if ((engineRPM >= ENGINE_RPM_MIN) && (engineRPM <= ENGINE_RPM_MAX)) {
			shiftRevMatch_.rpm = engineRPM;
			shiftRevMatch_.rpmNew = TRUE;
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

	if ((shiftRevMatch_.gear < GEAR_6) &&
		(shiftRevMatch_.rpmNew)) {
		const table_1d rpmTable = RpmThrottleMap[shiftRevMatch_.gear];

		if (rpmTable.x_length > 0U) {
			/* Translate target throttle position <- target RPM from interpolation */
			shiftRevMatch_.targetThrottle = Utils_interpolateTable1d(&rpmTable, shiftRevMatch_.rpm);

			/* Try to set Drive-By-Wire control from rev match */
			if (DBW_RevMatchSetControl(&shiftRevMatch_.targetThrottle) == REV_MATCH_DBW_OK) {
				/* DBW Control from rev match successful, start watchdog */
				GearWatchdog_Start(shiftRevMatch_.watchdog);
				/* Poll throttle target */
				nextState = REVMATCH_POLL_TARGET;
				/* Reset RPM data flag */
				shiftRevMatch_.rpmNew = FALSE;
			}
		}
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

		case REVMATCH_IDLE:
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
			(shiftRevMatch_.state == REVMATCH_IDLE));
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
bool_t ShiftRevMatch_IsFinished(void) {return TRUE; }
#endif // CONFIG_ENABLE_THROTTLE_BLIP

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

typedef struct {
	bool_t clutchEngaged;
	bool_t requested;
	bool_t rpmDataNew;
	bool_t wheelSpeedDataNew;
	uint32_t rpmData;
	uint32_t wheelSpeedData;
	GearStates gear;
	float targetRpm;
	float targetThrottle;
	GearWatchdogType *const watchdog;
} ShiftRevMatchHandler;

//! This must be before injectorsCutWdg declaration
static void ShiftRevMatchWatchdogElapsedTrigger(void);

//! Injectors Cut watchdog
static GearWatchdogType revMatchWdg = {
		.elapsedTrigger = ShiftRevMatchWatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = SHIFT_REVMATCH_WATCHDOG_TIMEOUT_MS
};

static ShiftRevMatchHandler shiftRevMatch_ = {
	.clutchEngaged = FALSE,
	.requested = FALSE,
	.rpmDataNew = FALSE,
	.wheelSpeedDataNew = FALSE,
	.rpmData = 0U,
	.wheelSpeedData = 0U,
	.gear = GEAR_UNKNOWN,
	.watchdog = &revMatchWdg
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
#if REVMATCH_FROM_ENGINE_RPM
static void ShiftRevMatch_ThrottleBlipEngineRPM(void);
static inline void ShiftRevMatch_CAN_GetEngineRPM(void);
#endif // REVMATCH_FROM_ENGINE_RPM

#if REVMATCH_FROM_WHEEL_RPM
static inline void ShiftRevMatch_CAN_GetWheelRPM(void);
static void ShiftRevMatch_ThrottleBlipWheelRPM(void);
#endif // REVMATCH_FROM_WHEEL_RPM
/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */
static void ShiftRevMatchWatchdogElapsedTrigger(void)
{
	/* Restore normal DBW operation - from throttle pedal position */
	DBW_RevMatchRestoreNormalOperation();
}

#if REVMATCH_FROM_ENGINE_RPM
static inline void ShiftRevMatch_CAN_GetEngineRPM(void)
{
	uint8_t *const emuBlackMsgBuff = CAN_GetRxNewData(CAN_RX_MSG_EMU_BLACK);

	/* Read CAN data engine RPM's only when buffer is valid and clutch is not engaged */
	if ((emuBlackMsgBuff != NULL) && (!ClutchControl_IsEngaged())) {
		/* Get engine RPM from EMU Black */
		const uint32_t engineRPM = (uint32_t)emuBlackMsgBuff[CAN_DATA_BYTE_0];

		if ((engineRPM >= ENGINE_RPM_MIN) && (engineRPM <= ENGINE_RPM_MAX)) {
			shiftRevMatch_.rpmData = engineRPM;
			shiftRevMatch_.rpmDataNew = TRUE;
		}
	}
}

static void ShiftRevMatch_ThrottleBlipEngineRPM(void)
{
	if ((shiftRevMatch_.rpmDataNew) &&
		(shiftRevMatch_.gear < GEAR_6)) {
		/* Get map Wheel RPM <- Gear */
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
				shiftRevMatch_.rpmDataNew = FALSE;
				GearWatchdog_Start(shiftRevMatch_.watchdog);
			}
		}
	}
}
#endif // REVMATCH_FROM_ENGINE_RPM

#if REVMATCH_FROM_WHEEL_RPM
static inline void ShiftRevMatch_CAN_GetWheelRPM(void)
{
	uint8_t *const telemetryMsgBuff = CAN_GetRxNewData(CAN_RX_MSG_TELEMETRY);

	if (telemetryMsgBuff != NULL) {
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
#endif // REVMATCH_FROM_WHEEL_RPM

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

void ShiftRevMatch_Trigger(const GearStates revMatchGear)
{
	if (revMatchGear < GEAR_6) {
		shiftRevMatch_.gear = revMatchGear;
		shiftRevMatch_.requested = TRUE;
	}
}

void ShiftRevMatch_Process(void)
{
#if REVMATCH_FROM_ENGINE_RPM
	/* Get Engine RPM from CAN (if selected from config) */
	ShiftRevMatch_CAN_GetEngineRPM();

	if (shiftRevMatch_.requested) {
		ShiftRevMatch_ThrottleBlipEngineRPM();
	}
#endif // REVMATCH_FROM_ENGINE_RPM

#if REVMATCH_FROM_WHEEL_RPM
	/* Get Wheel RPM from CAN (if selected from config) */
	ShiftRevMatch_CAN_GetWheelRPM();

	if (shiftRevMatch_.requested) {
		ShiftRevMatch_ThrottleBlipWheelRPM();
	}
#endif // REVMATCH_FROM_WHEEL_RPM
}
#else
void ShiftRevMatch_Trigger(const GearStates revMatchGear) { (void)revMatchGear; }
void ShiftRevMatch_Process(void) {}
ErrorEnum ShiftRevMatch_Init(void) { return ERROR_OK; }
#endif // CONFIG_ENABLE_REV_MATCH

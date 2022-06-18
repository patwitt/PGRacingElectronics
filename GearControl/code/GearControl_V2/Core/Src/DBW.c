/*
 * DBW.c
 *
 *  Created on: 22.05.2022
 *      Author: Patryk Wittbrodt
 */

#include "DefineConfig.h"
#include "GearControl.h"
#include "GearSensor.h"
#include "main.h"
#include "stdlib.h"
#include "Adc.h"
#include "SwTimer.h"
#include "DCMotorController.h"
#include "PID.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
/*
 * CLOSED
 * TPS1 - 3522
 * TPS2 - 730-732
 *
 * OPENED
 * TPS 1 - 730
 * TPS 2 - 4070
 */

#define APPS_CALIBRATION_TIME_MS (5000U)
#define TPS_CALIBRATION_TIME_MS (5000U)

#define ADC_MAX (4096U)
#define TPS_ADC_MAX_DIFF_THRESHOLD (400U)  // 10% of 5V
#define APPS_ADC_MAX_DIFF_THRESHOLD (400U) // 10% of 5V

#define TPS_DEBOUNCE_MS (100U)
#define APPS_DEBOUNCE_MS (100U)

#define TPS_INIT_DELAY_MS (100U)
#define TPS_INIT_CALIBRATION_MS (500U)
#define TPS_INIT_IDLE_EXPECTED (733U)


#define APPS_MIN_MEASURED (723U)
#define APPS_MAX_MEASURED (3196U)
#define APPS_MIN_MEASURED_F (723.0f)
#define APPS_MAX_MEASURED_F (3196.0f)
#define APPS_CONVERTED_MAX_F (1000.0f)
#define APPS_MIN_MAX_DIFF_F (APPS_MAX_MEASURED_F - APPS_MIN_MEASURED_F)
#define APPS_DIVISOR_F (APPS_CONVERTED_MAX_F / APPS_MIN_MAX_DIFF_F)
#define APPS_POS_MAX (1000.0f)
#define APPS_POS_MIN (0.0f)
#define APPS_DIVISOR(min, max) (APPS_CONVERTED_MAX_F / (max - min))


#define TPS_MIN_MEASURED_F (730.0f)
#define TPS_MAX_MEASURED_F (3500.0f)
#define TPS_MIN_MAX_DIFF_F (TPS_MAX_MEASURED_F - TPS_MIN_MEASURED_F)
#define TPS_POS_MAX (1000.0f)
#define TPS_POS_MIN (0.0f)
#define TPS_DIVISOR(min, max) (TPS_MIN_MAX_DIFF_F / (max - min))

typedef enum
{
	DBW_INIT,
	DBW_RUN,
	DBW_CALIBRATE_APPS,
	DBW_DISABLED
} DBW_States;

typedef struct {
	const uint16 maxDiffAllowed;
	const uint8_t debounceMs;
	const ErrorEnum errorFlag;
	uint16_t absDiff;
	uint16_t maxAbsDiff;
	uint8_t debounceCnt;
} PlausibilityParamType;

typedef struct {
	/* TPS1 ADC Channel */
	__IO AdcDataChannel* tps1;
	/* TPS2 ADC Channel */
	__IO AdcDataChannel* tps2;
	/* TPS ADC in IDLE position */
	uint16 constTpsIdle;
	/* IDLE min */
	uint16 idlePosMin;
	/* IDLE max */
	uint16 idlePosMax;
	float max;
	float min;
	float position; /* TPS position 0-1000 -> 0.1% of max range (around 50deg) */
	PlausibilityParamType *const plausibility;
	ErrorEnum error;
	SwTimerType timer;
} TpsSensorType;

typedef struct {
	/* APPS1 ADC Channel */
	__IO AdcDataChannel* apps1;
	/* APPS2 ADC Channel */
	__IO AdcDataChannel* apps2;
	ErrorEnum error;
	PlausibilityParamType *const plausibility;
	float max;
	float min;
	float target; /* Target position 0-1000 -> 0.1% of max range (around 50deg) */
	SwTimerType timer;
} AppsSensorType;

typedef struct
{
	TpsSensorType *const tps;
	AppsSensorType *const apps;
	/* DBW state */
	DBW_States state;
} DbwHandle;

static PlausibilityParamType tpsPlausibility = {.maxAbsDiff = 0U, .maxDiffAllowed = TPS_ADC_MAX_DIFF_THRESHOLD, .debounceMs = TPS_DEBOUNCE_MS, .errorFlag = ERROR_DBW_TPS_PLAUSIBILITY};
static PlausibilityParamType appsPlausibility = {.maxAbsDiff = 0U, .maxDiffAllowed = APPS_ADC_MAX_DIFF_THRESHOLD, .debounceMs = APPS_DEBOUNCE_MS, .errorFlag = ERROR_DBW_APPS_PLAUSIBILITY};

/* Sensors */
static TpsSensorType tps_ = {.tps1 = NULL, .tps2 = NULL, .idlePosMin = 0xFFFFU, .idlePosMax = 0U, .error = ERROR_OK, .plausibility = &tpsPlausibility};
static AppsSensorType apps_ = {.apps1 = NULL, .apps2 = NULL, .error = ERROR_OK, .plausibility = &appsPlausibility, .min = APPS_MIN_MEASURED_F, .max = APPS_MAX_MEASURED_F};

/* DBW */
static DbwHandle dbw = {.tps = &tps_, .apps = &apps_, .state = DBW_DISABLED};

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */
static float DBW_ConvertTpsRawValue(void);
static float DBW_ConvertAppsRawValue(void);

/* Process handlers */
static DBW_States DBW_Handler_Init(void);
static DBW_States DBW_Handler_Run(void);
static DBW_States DBW_Handler_calibrateAPPS(void);

/* Plausibility */
static void DBW_PlausibilityCheck(PlausibilityParamType *const plausibility, const uint16 sens1, const uint16 sens2, ErrorEnum *const error);

ErrorEnum DBW_Init(void)
{
	ErrorEnum err = ERROR_OK;

	tps_.tps1 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_TPS_1);
	tps_.tps2 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_TPS_2);
	apps_.apps1 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_APPS_1);
	apps_.apps2 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_APPS_2);

	if ((tps_.tps1 != NULL) && (tps_.tps1->raw != NULL) &&
		(tps_.tps2 != NULL) && (tps_.tps2->raw != NULL) &&
		(apps_.apps1 != NULL) && (apps_.apps1->raw != NULL) &&
		(apps_.apps2 != NULL) && (apps_.apps2->raw != NULL)) {

		err = SwTimerInit(&apps_.timer, APPS_CALIBRATION_TIME_MS, FALSE);
		if (err == ERROR_OK) {
			err = SwTimerRegister(&apps_.timer);
		}

		if (err == ERROR_OK) {
			err = SwTimerInit(&tps_.timer, TPS_CALIBRATION_TIME_MS, FALSE);
			if (err == ERROR_OK) {
				err = SwTimerRegister(&tps_.timer);
			}
		}

		if (err == ERROR_OK) {
			if (DCMotorController_Init() == ERROR_OK) {
				dbw.state = DBW_INIT;
			} else {
				err = ERROR_DBW_DC_MOTOR_INIT;
			}
		}
	} else {
		err = ERROR_NULL;
	}

	return err;
}

void DBW_RequestAppsCalibration(void) {
	if (dbw.state != DBW_DISABLED) {
		DCMotorController_Disable();
		SwTimerStart(&apps_.timer);
		apps_.min = 0xFFFFU;
		apps_.max = 0U;
		dbw.state = DBW_CALIBRATE_APPS;
	}
}

static DBW_States DBW_Handler_Init(void)
{
	DBW_States nextState = DBW_INIT;

	if (HAL_GetTick() > TPS_INIT_DELAY_MS) {
		/* Get idle min/max values */
		if (tps_.tps2->avg > tps_.idlePosMax) {
			tps_.idlePosMax = tps_.tps2->avg;
		}

		if (tps_.tps2->avg < tps_.idlePosMin) {
			tps_.idlePosMin = tps_.tps2->avg;
		}

		/* TPS IDLE calibration */
		if (HAL_GetTick() > (TPS_INIT_DELAY_MS + TPS_INIT_CALIBRATION_MS)) {
			nextState = DBW_DISABLED;

			// 728/735
			if (abs(tps_.idlePosMax - tps_.idlePosMin) < 10U) {
				tps_.constTpsIdle = (uint16)((tps_.idlePosMax + tps_.idlePosMin) / 2U);

				if (abs(TPS_INIT_IDLE_EXPECTED - tps_.constTpsIdle) < 10U) {
					/* Init OK */
					nextState = DBW_CALIBRATE_APPS;
					DBW_RequestAppsCalibration();
				} else {
					tps_.error = ERROR_DBW_TPS_INIT;
				}
			} else {
				tps_.error = ERROR_DBW_TPS_INIT;
			}
		}
	}

	return nextState;
}

static DBW_States DBW_Handler_Run(void)
{
	float position = DBW_ConvertTpsRawValue();
	float target = DBW_ConvertAppsRawValue();

	float pidOut = PID_Update(target, position);

	if (target > position) {
		DCMotorController_Update(pidOut, DC_MOTOR_ROTATE_PLUS);
	} else {
		DCMotorController_Update(pidOut, DC_MOTOR_ROTATE_MINUS);
	}

	return DBW_RUN;
}

static DBW_States DBW_Handler_calibrateAPPS(void) {
	DBW_States nextState = DBW_CALIBRATE_APPS;

	if (!SwTimerHasTimerElapsed(&apps_.timer)) {
		if (*apps_.apps2->raw < apps_.min) {
			apps_.min = *apps_.apps2->raw;
		}
		if (*apps_.apps2->raw > apps_.max) {
			apps_.max = *apps_.apps2->raw;
		}
	} else {
		if ((uint16_t)abs(ADC_MAX - (apps_.min + apps_.max)) < APPS_ADC_MAX_DIFF_THRESHOLD) {
			/* Calibration OK */
			DCMotorController_Enable();
			nextState = DBW_RUN;
		} else {
			nextState = DBW_DISABLED;
			apps_.error = ERROR_DBW_APPS_INIT;
		}
	}

	return nextState;
}

void DBW_Disable(boolean isError)
{
	/* Disable DBW */
	if (isError) {
		dbw.state = DBW_DISABLED;
	}
	DCMotorController_Disable();
}

void DBW_Process(void)
{
	switch (dbw.state)
	{
		case DBW_INIT:
			dbw.state = DBW_Handler_Init();
			break;

		case DBW_CALIBRATE_APPS:
			dbw.state = DBW_Handler_calibrateAPPS();
			break;

		case DBW_RUN:
			DBW_PlausibilityCheck(tps_.plausibility, *tps_.tps1->raw, *tps_.tps2->raw, &tps_.error);
			DBW_PlausibilityCheck(apps_.plausibility, *apps_.apps1->raw, *apps_.apps2->raw, &apps_.error);

			if ((apps_.error == ERROR_OK) && (tps_.error == ERROR_OK)) {
				dbw.state = DBW_Handler_Run();
			} else {
				dbw.state = DBW_DISABLED;
			}
			break;

		case DBW_DISABLED:
		default:
			DCMotorController_Disable();
			break;
	}
}

static float DBW_ConvertTpsRawValue(void)
{
	float tpsPos = TPS_POS_MIN;

	if (*tps_.tps2->raw >= tps_.min) {
		tpsPos = ((float)*tps_.tps2->raw - tps_.min) * APPS_DIVISOR(tps_.min, tps_.max);
		tpsPos = CLAMP_MAX(tpsPos, TPS_POS_MAX);
	}

	return tpsPos;
}

static float DBW_ConvertAppsRawValue(void) {
	float targetApps = APPS_POS_MIN;

	if (*apps_.apps2->raw >= apps_.min) {
		targetApps = ((float)*apps_.apps2->raw - apps_.min) * APPS_DIVISOR(apps_.min, apps_.max);
		targetApps = CLAMP_MAX(targetApps, APPS_POS_MAX);
	}

	return targetApps;
}

static void DBW_PlausibilityCheck(PlausibilityParamType *const plausibility, const uint16 sens1, const uint16 sens2, ErrorEnum *const error)
{
	plausibility->absDiff = (uint16_t)abs(ADC_MAX - (sens1 + sens2));

	if (plausibility->absDiff > plausibility->maxAbsDiff) {
		plausibility->maxAbsDiff = plausibility->absDiff;
	}

	if ((plausibility->absDiff) < plausibility->maxDiffAllowed) {
		if (plausibility->debounceCnt > 0U) {
			--plausibility->debounceCnt;
		}
	} else {
		if (*error == ERROR_OK) {
			++plausibility->debounceCnt;
		}
	}

	if (plausibility->debounceCnt == 0U) {
		*error = ERROR_OK;
	}
	else if (plausibility->debounceCnt > plausibility->debounceMs) {
		*error = plausibility->errorFlag;
	}
	else { /* Nothing */ }
}

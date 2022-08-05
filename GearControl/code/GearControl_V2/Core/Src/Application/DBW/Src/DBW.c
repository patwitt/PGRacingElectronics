/*
 * DBW.c
 *
 *  Created on: 22.05.2022
 *      Author: Patryk Wittbrodt
 */

#include "DBW.h"
#include "DCMotor.h"
#include "DefineConfig.h"
#include "Utils.h"
#include "main.h"
#include "stdlib.h"
#include "PID.h"
#include "Adc.h"
#include "LED.h"
#include "SwTimer.h"
#include "RCFilter.h"
#include "IIRFilter.h"

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
#define TPS_CALIBRATION_TIME_MS (1000U)

#define ADC_MAX (4096U)
#define TPS_ADC_MAX_DIFF_THRESHOLD (410U)  // 10% of 5V
#define APPS_ADC_MAX_DIFF_THRESHOLD (410U) // 10% of 5V

#define TPS_DEBOUNCE_MS (100U)
#define APPS_DEBOUNCE_MS (100U)

#define TPS_INIT_DELAY_MS (100U)
#define TPS_INIT_CALIBRATION_MS (500U)

#define APPS_MIN_MEASURED (720U) // 723
#define APPS_MAX_MEASURED (3500U) // 3196

#define APPS_FEASIBLE_MIN (800U)
#define APPS_FEASIBLE_MAX (3000U)
#define APPS_MIN_MEASURED_F (720.0f)
#define APPS_MAX_MEASURED_F (3200.0f)
#define APPS_POS_MAX_F (1000.0f)
#define APPS_POS_MIN_F (0.0f)
#define APPS_DIVISOR_F(min, max) (APPS_POS_MAX_F / (max - min))


#define TPS_MIN_CALIBRATION_PLAUSIBILITY_SAMPLES (50U)
#define TPS_FEASIBLE_MIN (800U)
#define TPS_FEASIBLE_MAX (3600U)
#define TPS_CALIBRATION_SPEED (380.0f) // 0-1000
#define TPS_MIN_MEASURED_F (730.0f)
#define TPS_MAX_MEASURED_F (3700.0f)
#define TPS_POS_MAX_F (1000.0f)
#define TPS_POS_MIN_F (0.0f)
#define TPS_DIVISOR_F(min, max) (TPS_POS_MAX_F / (max - min))

#define DBW_PWM_SPRING_MIN (300.0f)

/* Filter constants */
#define LPF_RC_CUTOFF_FREQ_HZ (50.0f)
#define LPF_RC_TIME_CONSTANT_S (0.001f)
#define IIR_FILTER_ALPHA (0.5f)

typedef enum
{
	DBW_INIT,
	DBW_RUN,
	DBW_CALIBRATE_APPS,
	DBW_CALIBRATE_TPS,
	DBW_DISABLED
} DBW_States;

typedef struct {
	float max;
	float min;
	uint16 calibMin;
	uint16 calibMax;
	const uint16 feasibleMin;
	const uint16 feasibleMax;
} SensorLimitsType;

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

	float position; /* TPS position 0-1000 -> 0.1% of max range (around 50deg) */
	DCMotorDirectionEnum calibrationDirection;
	PlausibilityParamType *const plausibility;
	SensorLimitsType *const limits;
	ErrorEnum error;
	SwTimerType timer;
	uint16 calibOkCnt;
	uint16 calibNokCnt;
	RCFilter rcFilter;
	IIRFilter iirFilter;
	float posMin;
	float posMax;
} TpsSensorType;

typedef struct {
	/* APPS1 ADC Channel */
	__IO AdcDataChannel* apps1;
	/* APPS2 ADC Channel */
	__IO AdcDataChannel* apps2;
	ErrorEnum error;
	PlausibilityParamType *const plausibility;
	SensorLimitsType *const limits;
	float target; /* Target position 0-1000 -> 0.1% of max range (around 50deg) */
	SwTimerType timer;
	RCFilter rcFilter;
	IIRFilter iirFilter;
	float posMin;
	float posMax;
} AppsSensorType;

typedef struct
{
	TpsSensorType *const tps;
	AppsSensorType *const apps;
	/* DBW state */
	DBW_States state;
} DbwHandle;

/* Plausibility */
static PlausibilityParamType tpsPlaus = {.maxAbsDiff = 0U, .maxDiffAllowed = TPS_ADC_MAX_DIFF_THRESHOLD, .debounceMs = TPS_DEBOUNCE_MS, .errorFlag = ERROR_DBW_TPS_PLAUSIBILITY};
static PlausibilityParamType appsPlaus = {.maxAbsDiff = 0U, .maxDiffAllowed = APPS_ADC_MAX_DIFF_THRESHOLD, .debounceMs = APPS_DEBOUNCE_MS, .errorFlag = ERROR_DBW_APPS_PLAUSIBILITY};

/* Limits */
static SensorLimitsType tpsLim = {.min = TPS_MIN_MEASURED_F, .max = TPS_MAX_MEASURED_F, .calibMax = 0U, .calibMin = 0xFFFFU, .feasibleMin = TPS_FEASIBLE_MIN, .feasibleMax = TPS_FEASIBLE_MAX};
static SensorLimitsType appsLim = {.min = APPS_MIN_MEASURED_F, .max = APPS_MAX_MEASURED_F, .calibMax = 0U, .calibMin = 0xFFFFU, .feasibleMin = APPS_FEASIBLE_MIN, .feasibleMax = APPS_FEASIBLE_MAX};

/* Sensors */
static TpsSensorType tps_ = {.tps1 = NULL, .tps2 = NULL, .idlePosMin = 0xFFFFU, .idlePosMax = 0U, .error = ERROR_OK, .plausibility = &tpsPlaus, .limits = &tpsLim, .calibrationDirection = DC_MOTOR_DISABLED, .posMin = 1001.0f, .posMax = 0.0f};
static AppsSensorType apps_ = {.apps1 = NULL, .apps2 = NULL, .error = ERROR_OK, .plausibility = &appsPlaus, .limits = &appsLim, .posMin = 1001.0f, .posMax = 0.0f};

/* DBW */
static DbwHandle dbw = {.tps = &tps_, .apps = &apps_, .state = DBW_DISABLED};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
/* Convert raw ADC to Float */
static inline float DBW_ConvertTpsRawValue(void);
static inline float DBW_ConvertAppsRawValue(void);

/* Process handlers */
static DBW_States DBW_HandlerInit(void);
static DBW_States DBW_HandlerRun(void);
static DBW_States DBW_HandlerCalibrateAPPS(void);
static DBW_States DBW_HandlerCalibrateTPS(void);

/* Plausibility */
static void DBW_PlausibilityCheck(PlausibilityParamType *const plausibility, SensorLimitsType *const limits, const uint16 sens1, const uint16 sens2, ErrorEnum *const error);
#if CONFIG_DBW_ADJUST_SENS_LIMITS
static void DBW_AdjustSensorLimits(SensorLimitsType *const limits, const uint16 invalidValue);
#endif
/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static DBW_States DBW_HandlerInit(void)
{
	DBW_States nextState = DBW_INIT;

	if (HAL_GetTick() > TPS_INIT_DELAY_MS) {
		/* Get idle min/max values */
		Utils_UpdateMinMax_U16(*tps_.tps2->raw, &tps_.idlePosMin, &tps_.idlePosMax);
		/* TPS IDLE calibration */
		if (HAL_GetTick() > (TPS_INIT_DELAY_MS + TPS_INIT_CALIBRATION_MS)) {
			nextState = DBW_DISABLED;

			// 728/735
			if (abs(tps_.idlePosMax - tps_.idlePosMin) < 20U) {
				tps_.constTpsIdle = (uint16)((tps_.idlePosMax + tps_.idlePosMin) / 2U);
					/* Init OK */
					DCMotor_Enable();

#if CONFIG_DBW_CALIBRATE_TPS_AUTO
					/* Start TPS calibration */
					SwTimerStart(&tps_.timer, TPS_CALIBRATION_TIME_MS);
					tps_.calibrationDirection = DC_MOTOR_ROTATE_PLUS;
					DCMotor_Update(TPS_CALIBRATION_SPEED, tps_.calibrationDirection);
					tps_.limits->calibMax = 0U;
					tps_.limits->calibMin = 0xFFFFU;
					tps_.calibOkCnt = 0U;
					tps_.calibNokCnt = 0U;
					nextState = DBW_CALIBRATE_TPS;
#else
					nextState = DBW_RUN;
					LED_SetStatus(LED_SOLID);
#endif
			} else {
				tps_.error = ERROR_DBW_TPS_INIT;
			}
		}
	}

	return nextState;
}

static DBW_States DBW_HandlerCalibrateAPPS(void)
{
	DBW_States nextState = DBW_CALIBRATE_APPS;
	apps_.plausibility->absDiff = (uint16_t)abs(ADC_MAX - (*apps_.apps1->raw + *apps_.apps2->raw));
	Utils_UpdateMax_U16(apps_.plausibility->absDiff, &apps_.plausibility->maxAbsDiff);

	if (!SwTimerHasElapsed(&apps_.timer)) {
		if (apps_.plausibility->absDiff < apps_.plausibility->maxDiffAllowed) {
			Utils_UpdateMinMax_U16(*apps_.apps2->raw, &apps_.limits->calibMin, &apps_.limits->calibMax);
		}
	} else {
		if ((apps_.plausibility->absDiff < apps_.plausibility->maxDiffAllowed) &&
		    (apps_.limits->calibMin < APPS_FEASIBLE_MIN) &&
		    (apps_.limits->calibMax > APPS_FEASIBLE_MAX)) {
			/* Calibration OK */
			apps_.limits->min = (float)apps_.limits->calibMin;
			apps_.limits->max = (float)apps_.limits->calibMax;
			DCMotor_Enable();
			nextState = DBW_RUN;
		} else {
			nextState = DBW_DISABLED;
			apps_.error = ERROR_DBW_APPS_CALIBRATION;
		}
	}

	return nextState;
}

static DBW_States DBW_HandlerCalibrateTPS(void)
{
	DBW_States nextState = DBW_CALIBRATE_TPS;

	tps_.plausibility->absDiff = (uint16_t)abs(ADC_MAX - (*tps_.tps2->raw + *tps_.tps1->raw));
	Utils_UpdateMax_U16(tps_.plausibility->absDiff, &tps_.plausibility->maxAbsDiff);

	if (!SwTimerHasElapsed(&tps_.timer)) {
		/* Run TPS calibration to set new MIN/MAX ADC values */
		const bool_t isPlausible = (tps_.plausibility->absDiff < tps_.plausibility->maxDiffAllowed);

		if (isPlausible) {
			++tps_.calibOkCnt;
			/* Continue going UP/DOWN */
			Utils_UpdateMinMax_U16(*tps_.tps2->raw, &tps_.limits->calibMin, &tps_.limits->calibMax);
		} else {
			/* Invalid plausibility, reached end */
			if (tps_.calibNokCnt > tps_.calibOkCnt) {
				if (tps_.calibNokCnt > TPS_MIN_CALIBRATION_PLAUSIBILITY_SAMPLES) {
					nextState = DBW_DISABLED;
					tps_.error = ERROR_DBW_TPS_CALIBRATION;
				}
			}
			++tps_.calibNokCnt;
		}

		switch (tps_.calibrationDirection) {
			case DC_MOTOR_ROTATE_PLUS:
				 if ((!isPlausible) && ((tps_.calibOkCnt > TPS_MIN_CALIBRATION_PLAUSIBILITY_SAMPLES) || (*tps_.tps2->raw > 4000U))) {
						/* UP calibration finished successfully, change direction */
						tps_.calibNokCnt = 0U;
						tps_.calibOkCnt = 0U;
						tps_.calibrationDirection = DC_MOTOR_ROTATE_MINUS;
						DCMotor_Update(TPS_CALIBRATION_SPEED, tps_.calibrationDirection);
				}
				break;

			case DC_MOTOR_ROTATE_MINUS:
				if ((!isPlausible) && (tps_.calibOkCnt > TPS_MIN_CALIBRATION_PLAUSIBILITY_SAMPLES)) {
					/* DOWN calibration finished successfully */
					tps_.calibrationDirection = DC_MOTOR_DISABLED;
					DCMotor_Disable();
				}
				break;

			case DC_MOTOR_DISABLED:
			default:
				DCMotor_Disable();
				break;
		}
	} else {
		/* Timer elapsed, calibration finished */
		if ((tps_.limits->calibMin < TPS_FEASIBLE_MIN) &&
			(tps_.limits->calibMax > TPS_FEASIBLE_MAX)) {
			/* Calibration OK */
			DCMotor_Enable();
			tps_.limits->min = (float)tps_.limits->calibMin;
			tps_.limits->max = (float)tps_.limits->calibMax;
			LED_SetStatus(LED_SOLID);
			nextState = DBW_RUN;
		} else {
			nextState = DBW_DISABLED;
			tps_.error = ERROR_DBW_TPS_CALIBRATION;
		}
	}

	return nextState;
}

static DBW_States DBW_HandlerRun(void)
{
	tps_.position = DBW_ConvertTpsRawValue();
	apps_.target = DBW_ConvertAppsRawValue();
#if CONFIG_PID_ENABLE_RC_LPF
	/* Low-Pass Filter on samples */
	tps_.position = RCFilter_Update(&tps_.rcFilter, tps_.position);
	apps_.target = RCFilter_Update(&apps_.rcFilter, apps_.target);
#elif CONFIG_PID_ENABLE_IIR
	tps_.position = IIRFilter_Update(&tps_.iirFilter, tps_.position);
	apps_.target = IIRFilter_Update(&apps_.iirFilter, apps_.target);
#endif
	
	apps_.target = CLAMP_MIN(apps_.target, TPS_IDLE);

#if CONFIG_ADC_SHOW_MIN_MAX
	static bool_t startPosMeas = FALSE;
	if (tps_.position > 500.0f) {
		startPosMeas = TRUE;
	}

	if (startPosMeas) {
		Utils_UpdateMinMax_F(tps_.position, &tps_.posMin, &tps_.posMax);
	}
#endif
	/* Update PID output */
	float pidOut = PID_Update(&apps_.target, tps_.position);

	/* Direction */
	bool_t direction = (pidOut >= 0.0f);
	pidOut = (direction) ? pidOut : (pidOut * (-1.0f));

	switch ((uint8)direction) {
		case TRUE:
			DCMotor_Update(pidOut, DC_MOTOR_ROTATE_PLUS);
			break;

		case FALSE:
			DCMotor_Update(pidOut, DC_MOTOR_ROTATE_MINUS);
			break;

		default:
			DCMotor_Update(0U, DC_MOTOR_DISABLED);
			break;
	}

	return DBW_RUN;
}

static inline float DBW_ConvertTpsRawValue(void)
{
	float tpsPos = TPS_POS_MIN_F;

	if (tps_.tps2->avgData.avg >= tps_.limits->min) {
		tpsPos = ((float)tps_.tps2->avgData.avg - tps_.limits->min) * TPS_DIVISOR_F(tps_.limits->min, tps_.limits->max);
		tpsPos = CLAMP_MAX(tpsPos, TPS_POS_MAX_F);
	}

	return tpsPos;
}

static inline float DBW_ConvertAppsRawValue(void)
{
	float targetApps = APPS_POS_MIN_F;

	if (apps_.apps2->avgData.avg >= apps_.limits->min) {
		targetApps = ((float)apps_.apps2->avgData.avg - apps_.limits->min) * APPS_DIVISOR_F(apps_.limits->min, apps_.limits->max);
		targetApps = CLAMP_MAX(targetApps, APPS_POS_MAX_F);
	}

	return targetApps;
}
#if CONFIG_DBW_ADJUST_SENS_LIMITS
static void DBW_AdjustSensorLimits(SensorLimitsType *const limits, const uint16 invalidValue)
{
	if (invalidValue > limits->feasibleMax) {
		limits->max -= 1.0f;
	}
	else if (invalidValue < limits->feasibleMin) {
		limits->min += 1.0f;
	} else { /* Do nothing, actual error inside normal range */}
}
#endif
static void DBW_PlausibilityCheck(PlausibilityParamType *const plausibility, SensorLimitsType *const limits, const uint16 sens1, const uint16 sens2, ErrorEnum *const error)
{
	if (sens2 < limits->feasibleMax && sens2 > limits->feasibleMin) {
		plausibility->absDiff = (uint16_t)abs(ADC_MAX - (sens1 + sens2));
		Utils_UpdateMax_U16(plausibility->absDiff, &plausibility->maxAbsDiff);

		if ((plausibility->absDiff) < plausibility->maxDiffAllowed) {
			if (plausibility->debounceCnt > 0U) {
				--plausibility->debounceCnt;
			}
		} else {
#if CONFIG_DBW_ADJUST_SENS_LIMITS
			/* Actively adjust limits */
			DBW_AdjustSensorLimits(limits, sens2);
#endif
			if (*error == ERROR_OK) {
				++plausibility->debounceCnt;
			}
		}

		if (plausibility->debounceCnt == 0U) {
			*error = ERROR_OK;
		}
		else if (plausibility->debounceCnt > plausibility->debounceMs) {
			*error = plausibility->errorFlag;
		} else { /* Nothing */ }
	}
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
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

		err = SwTimerRegister(&apps_.timer);

		if (err == ERROR_OK) {
			err = SwTimerRegister(&tps_.timer);
		}

		if (err == ERROR_OK) {
			if (DCMotor_Init() == ERROR_OK) {
				dbw.state = DBW_INIT;

#if CONFIG_PID_ENABLE_RC_LPF
				/* Initialize Low-Pass Filters */
				err = RCFilter_Init(&apps_.rcFilter, LPF_RC_CUTOFF_FREQ_HZ, LPF_RC_TIME_CONSTANT_S);
				if (err == ERROR_OK) {
					err = RCFilter_Init(&tps_.rcFilter, LPF_RC_CUTOFF_FREQ_HZ, LPF_RC_TIME_CONSTANT_S);
				}
#elif CONFIG_PID_ENABLE_IIR
				/* Initialize IIR Filters */
				err = IIRFilter_Init(&apps_.iirFilter, IIR_FILTER_ALPHA);
				if (err == ERROR_OK) {
					err = IIRFilter_Init(&tps_.iirFilter, IIR_FILTER_ALPHA);
				}
#endif
			} else {
				err = ERROR_DBW_DC_MOTOR_INIT;
			}
		}
	} else {
		err = ERROR_NULL;
	}

	return err;
}

void DBW_Process(void)
{
	if (DCMotor_GetState() == DC_MOTOR_FAILURE) {
		dbw.state = DBW_DISABLED;
	}

	switch (dbw.state) {
		case DBW_INIT:
			dbw.state = DBW_HandlerInit();
			break;

		case DBW_CALIBRATE_APPS:
			dbw.state = DBW_HandlerCalibrateAPPS();
			break;

		case DBW_CALIBRATE_TPS:
			dbw.state = DBW_HandlerCalibrateTPS();
			break;

		case DBW_RUN:
			DBW_PlausibilityCheck(tps_.plausibility, tps_.limits, tps_.tps1->avgData.avg, tps_.tps2->avgData.avg, &tps_.error);
			DBW_PlausibilityCheck(apps_.plausibility, apps_.limits, apps_.apps1->avgData.avg, apps_.apps2->avgData.avg, &apps_.error);

			if ((apps_.error == ERROR_OK) && (tps_.error == ERROR_OK)) {
				dbw.state = DBW_HandlerRun();
			} else {
				dbw.state = DBW_DISABLED;
			}
			break;

		case DBW_DISABLED:
		default:
			DCMotor_Disable();
			LED_SetStatus(LED_BLINK_1HZ);
			break;
	}
}

void DBW_RequestAppsCalibration(void)
{
	if (dbw.state != DBW_DISABLED) {
		DCMotor_Disable();
		SwTimerStart(&apps_.timer, APPS_CALIBRATION_TIME_MS);
		apps_.limits->calibMin = 0xFFFFU;
		apps_.limits->calibMax = 0U;
		dbw.state = DBW_CALIBRATE_APPS;
	}
}

void DBW_Disable(boolean isError)
{
	/* Disable DBW */
	if (isError) {
		dbw.state = DBW_DISABLED;
	}
	DCMotor_Disable();
}
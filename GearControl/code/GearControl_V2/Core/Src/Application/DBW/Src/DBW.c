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

#define TPS_F_MIN_CALC_THRESHOLD (500.0f)
#define TPS_CALIBRATION_OOR (4000U)
#define TPS_IDLE_POS_MAX_DIFF (20U)
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
static PlausibilityParamType tpsPlaus = {
	.maxAbsDiff = 0U,
	.maxDiffAllowed = TPS_ADC_MAX_DIFF_THRESHOLD,
	.debounceMs = TPS_DEBOUNCE_MS,
	.errorFlag = ERROR_DBW_TPS_PLAUSIBILITY
};

static PlausibilityParamType appsPlaus = {
	.maxAbsDiff = 0U,
	.maxDiffAllowed = APPS_ADC_MAX_DIFF_THRESHOLD,
	.debounceMs = APPS_DEBOUNCE_MS,
	.errorFlag = ERROR_DBW_APPS_PLAUSIBILITY
};

/* Limits */
static SensorLimitsType tpsLim = {
	.min = TPS_MIN_MEASURED_F,
	.max = TPS_MAX_MEASURED_F,
	.calibMax = 0U,
	.calibMin = UINT16_MAX,
	.feasibleMin = TPS_FEASIBLE_MIN,
	.feasibleMax = TPS_FEASIBLE_MAX
};
static SensorLimitsType appsLim = {
	.min = APPS_MIN_MEASURED_F,
	.max = APPS_MAX_MEASURED_F,
	.calibMax = 0U,
	.calibMin = UINT16_MAX,
	.feasibleMin = APPS_FEASIBLE_MIN,
	.feasibleMax = APPS_FEASIBLE_MAX
};

/* Sensors */
static TpsSensorType tps_ = {
	.tps1 = NULL,
	.tps2 = NULL,
	.idlePosMin = UINT16_MAX,
	.idlePosMax = 0U,
	.error = ERROR_OK,
	.plausibility = &tpsPlaus,
	.limits = &tpsLim,
	.calibrationDirection = DC_MOTOR_DISABLED,
	.posMin = 1001.0f,
	.posMax = 0.0f
};

static AppsSensorType apps_ = {
	.apps1 = NULL,
	.apps2 = NULL,
	.error = ERROR_OK,
	.plausibility = &appsPlaus,
	.limits = &appsLim,
	.posMin = 1001.0f,
	.posMax = 0.0f
};

/* DBW */
static DbwHandle dbw = {
	.tps = &tps_,
	.apps = &apps_,
	.state = DBW_DISABLED
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
/* Convert raw ADC to Float */
static inline float DBW_ConvertTpsRawValue(void);
static inline float DBW_ConvertAppsRawValue(void);

/* Process handlers */
static void DBW_StateMachine(void);
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
/*        Local functions       */
/* ---------------------------- */

/**
 * @brief Drive-By-Wire Initialization state.
 * 
 * It waits for a certain amount of time, then it reads the TPS sensor and calculates the idle
 * position. 
 * If the idle position is within a certain range, it enables the DC motor and starts the calibration
 * process. 
 * If the idle position is not within the range, it sets an error flag.
 * 
 * @return The next state of the DBW_HandlerInit function.
 */
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
			if (abs(tps_.idlePosMax - tps_.idlePosMin) < TPS_IDLE_POS_MAX_DIFF) {
				tps_.constTpsIdle = (uint16)((tps_.idlePosMax + tps_.idlePosMin) / 2U);
					/* Init OK */
					DCMotor_Enable();

#if CONFIG_DBW_CALIBRATE_TPS_AUTO
					/* Start TPS calibration */
					SwTimerStart(&tps_.timer, TPS_CALIBRATION_TIME_MS);
					tps_.calibrationDirection = DC_MOTOR_ROTATE_PLUS;
					DCMotor_Update(TPS_CALIBRATION_SPEED, tps_.calibrationDirection);
					tps_.limits->calibMax = 0U;
					tps_.limits->calibMin = UINT16_MAX;
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

/**
 * @brief APPS sensor calibration sequence.
 * 
 * It checks if the difference between the two APPS sensors is within a certain threshold, and if so,
 * it updates the calibration limits.
 * 
 * @return The next state of the DBW_HandlerCalibrateAPPS function.
 */
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

/**
 * @brief TPS sensor calibration sequence.
 * 
 * The function is called every time the ADC interrupt is triggered. It checks if the difference
 * between the two ADC values is within a certain range. If it is, it updates the minimum and maximum
 * values. If it isn't, it checks if the difference is within the range for a certain number of times.
 * If it is, it changes the direction of the motor. If it isn't, it stops the motor. If the minimum and
 * maximum values are within a certain range, it enables the motor and changes the state to DBW_RUN. If
 * they aren't, it changes the state to DBW_DISABLED
 * 
 * @return The next state of the DBW_States enum.
 */
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
				 if ((!isPlausible) && ((tps_.calibOkCnt > TPS_MIN_CALIBRATION_PLAUSIBILITY_SAMPLES) || (*tps_.tps2->raw > TPS_CALIBRATION_OOR))) {
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

/**
 * @brief Drive-By-Wire PID runner.
 * 
 * The function reads the raw values from the ADC, filters them, and then updates the PID controller.
 * 
 * @return DBW_RUN state.
 */
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
	if (tps_.position > TPS_F_MIN_CALC_THRESHOLD) {
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

	static const DCMotorDirectionEnum directionToRotationMap[] = {
		[FALSE] = DC_MOTOR_ROTATE_MINUS,
		[TRUE] = DC_MOTOR_ROTATE_PLUS
	};

	DCMotor_Update(pidOut, directionToRotationMap[(uint8_t)direction]);

	return DBW_RUN;
}

/**
 * @brief Convert the raw TPS value to a percentage value between 0 and 1000.
 * 
 * The first thing we do is set the default value of tpsPos to TPS_POS_MIN_F. This is the default value
 * of 0.0.
 * 
 * @return Coverted TPS raw value to float.
 */
static inline float DBW_ConvertTpsRawValue(void)
{
	float tpsPos = TPS_POS_MIN_F;

	if (tps_.tps2->avgData.avg >= tps_.limits->min) {
		tpsPos = ((float)tps_.tps2->avgData.avg - tps_.limits->min) * TPS_DIVISOR_F(tps_.limits->min, tps_.limits->max);
		tpsPos = CLAMP_MAX(tpsPos, TPS_POS_MAX_F);
	}

	return tpsPos;
}

/**
 * @brief Convert the raw APPS value to a percentage value between 0 and 1000.
 * 
 * The first thing we do is set the default value of targetApps to TPS_POS_MIN_F. This is the default value
 * of 0.0.
 * 
 * @return Coverted APPS raw value to float.
 */
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
/**
 * @brief Adjust sensor limits dynamically.
 * 
 * If the invalid value is greater than the feasible max, then the max is decremented by 1.0f. If the
 * invalid value is less than the feasible min, then the min is incremented by 1.0f.
 * 
 * @param limits       Pointer to the limits structure.
 * @param invalidValue The value that is outside the feasible range.
 */
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
/**
 * @brief Drive-By-Wire sensors plausibility checks.
 * 
 * If the sensor value is within the feasible range, then check if the difference between the two
 * sensors is within the allowed range. If it is, then decrement the debounce counter. If it isn't,
 * then increment the debounce counter. If the debounce counter is zero, then there is no error. If the
 * debounce counter is greater than the debounce time, then there is an error.
 * 
 * @param plausibility Pointer to a struct of type PlausibilityParamType.
 * @param limits       The limits of the sensor.
 * @param sens1        The value of the first sensor reading.
 * @param sens2        The value of the second sensor reading.
 * @param error        The error flag to set if the plausibility check fails.
 */
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
/**
 * @brief Initialization of the Drive-By-Wire module.
 * 
 * It initializes the DBW module by initializing the ADC channels, the timers, the DC motor, and the
 * filters.
 * 
 * @return an error code.
 */
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

/**
 * @brief Drive-By-Wire main state machine.
 */
static void DBW_StateMachine(void)
{
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

/**
 * @brief Main process function that is called from the Scheduler.
 */
void DBW_Process(void)
{
	if (DCMotor_GetState() == DC_MOTOR_FAILURE) {
		dbw.state = DBW_DISABLED;
	}

	DBW_StateMachine();
}

/**
 * @brief Request APPS sensor calibration.
 * 
 * It starts a timer, sets the calibration limits to the maximum and minimum possible values, and sets
 * the state to calibrate the APPS.
 */
void DBW_RequestAppsCalibration(void)
{
	if (dbw.state != DBW_DISABLED) {
		DCMotor_Disable();
		SwTimerStart(&apps_.timer, APPS_CALIBRATION_TIME_MS);
		apps_.limits->calibMin = UINT16_MAX;
		apps_.limits->calibMax = 0U;
		dbw.state = DBW_CALIBRATE_APPS;
	}
}

/**
 * @brief Disable Drive-By-Wire.
 * 
 * If the DBW is enabled, disable it and set the state to disabled.
 * 
 * @param isError TRUE if error should be set, FALSE otherwise.
 */
void DBW_Disable(boolean isError)
{
	/* Disable DBW */
	if (isError) {
		dbw.state = DBW_DISABLED;
	}
	DCMotor_Disable();
}

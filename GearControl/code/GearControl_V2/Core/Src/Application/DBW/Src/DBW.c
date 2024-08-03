/*
 * DBW.c
 *
 *  Created on: 22.05.2022
 *      Author: Patryk Wittbrodt
 */

#include "DBW.h"
#include "DBWUtils.h"
#include "DBWTypes.h"
#include "DBWSafety.h"
#include "DBWCalibration.h"

#if CONFIG_ENABLE_DBW
#include "DCMotor.h"
#include "Utils.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "PID.h"
#include "Adc.h"
#include "LED.h"
#include "SwTimer.h"
#include "RCFilter.h"
#include "IIRFilter.h"
#include "KalmanFilter.h"

#include <math.h>

#if CONFIG_MAP_RPM_TEST
#include "DBWTest.h"
#endif

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

/* Plausibility */
static DBWSafety_PlausibilityType tpsPlaus = {
	.maxAbsDiff = 0U,
	.maxDiffAllowed = TPS_ADC_MAX_DIFF_THRESHOLD,
	.debounceMs = TPS_DEBOUNCE_MS,
	.errorFlag = ERROR_DBW_TPS_PLAUSIBILITY
};

static DBWSafety_PlausibilityType appsPlaus = {
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
	.posMax = 0.0f,
	.kalman = {
		.P_k_k1 = 1.0f,
		.Q = 0.001f,
		.R = 0.005f
	}
};

static AppsSensorType apps_ = {
	.apps1 = NULL,
	.apps2 = NULL,
	.error = ERROR_OK,
	.plausibility = &appsPlaus,
	.limits = &appsLim,
	.posMin = 1001.0f,
	.posMax = 0.0f,
	.kalman = {
		.P_k_k1 = 1.0f,
		.Q = 0.001f,
		.R = 0.005f
	}
};

static DBWSafety_TriggerHandler safetyTrigger_ = {
	    .gpioPort = GPIOB,
		.gpioPin  = GPIO_PIN_12,
		.safetyCnt = 0U,
		.status = true
};

/* DBW */
static DbwHandle dbw_ = {
	.errPosPercent = 0.0f,
	.tps = &tps_,
	.apps = &apps_,
#if CONFIG_ENABLE_THROTTLE_BLIP
	.revMatchControl = FALSE,
	.revMatchTarget = NULL,
#endif
	.state = DBW_DISABLED,
	.safetyTrigger = &safetyTrigger_,
	.apps_calib_request = FALSE
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */

/* Set Target */
static inline void DBW_SetTarget(void);
static inline void DBW_UpdateAppsTps(void);
#if CONFIG_ENABLE_THROTTLE_BLIP
static inline bool_t DBW_IsRevMatchTargetInRange(const float target);
#endif

/* Process handlers */
static void DBW_StateMachine(void);
static DBW_States DBW_HandlerInit(void);
static DBW_States DBW_HandlerRun(void);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */

static inline void DBW_SetNormalApps(void)
{
	apps_.target = DBWUtils_ConvertAppsRaw(&apps_);
	DBWUtils_FilterSensor(&apps_.target, &apps_.kalman, &apps_.rcFilter, &apps_.iirFilter);
#if CONFIG_DBW_APPS_NONLINEAR
	apps_.target = pow(apps_.target / APPS_POS_MAX_F, APPS_GAMMA_F) * APPS_POS_MAX_F;
#endif
}

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
		Utils_UpdateMinMax_U16(tps_.tps2->avgData.avg, &tps_.idlePosMin, &tps_.idlePosMax);
		/* TPS IDLE calibration */
		if (HAL_GetTick() > (TPS_INIT_DELAY_MS + TPS_INIT_CALIBRATION_MS)) {
			nextState = DBW_DISABLED_UNRECOVERABLE;

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

#if CONFIG_ENABLE_THROTTLE_BLIP
/**
 * @brief Get absolute error between target and throttle position.
 * 
 * @return The absolute value of the difference between the target position and the current position.
 */
float DBW_GetTargetPositionAbsError(void)
{
	const float targetErr = apps_.target - tps_.position;

	return (targetErr > 0.0f) ? targetErr : (targetErr * (-1.0f));
}

/**
 * @brief Check if throttle target is in range.
 * 
 * @param target The throttle target value.
 * 
 * @return TRUE if throttle target is in range, FALSE otherwise.
 */
static inline bool_t DBW_IsRevMatchTargetInRange(const float target)
{
#define TARGET_REVMATCH_MIN (200.0f)
#define TARGET_REVMATCH_MAX (1000.0f)
	return ((target >= TARGET_REVMATCH_MIN) &&
			(target <= TARGET_REVMATCH_MAX));
}

/**
 * @brief Set RevMatch Control if possible.
 * 
 * If the DBW is running, the TPS and APPS are OK, and the target is in range,
 * then set the target from revmatch module and enable the control.
 * 
 * @param target The throttle target value from rev match module.
 * 
 * @return a DbwRevMatchStatus enum.
 */
DbwRevMatchStatus DBW_RevMatchSetControl(float *const target)
{
	DbwRevMatchStatus revMatchStatus = REV_MATCH_DBW_OK;

	if ((dbw_.state == DBW_RUN) &&
	    (tps_.error == ERROR_OK) &&
	    (apps_.error == ERROR_OK)) {
		if (NULL_CHECK1(target) &&
			DBW_IsRevMatchTargetInRange(*target)) {
				dbw_.revMatchTarget = target;
				dbw_.revMatchControl = TRUE;
		} else {
			revMatchStatus = REV_MATCH_TARGET_INVALID;
		}
	} else {
		revMatchStatus = REV_MATCH_DBW_FAILURE;
	}

	return revMatchStatus;
}

/**
 * @brief Restore normal Drive-by-Wire operation from throttle pedal position.
 * 
 * This is called after revmatching sequence is finished.
 * 
 */
void DBW_RevMatchRestoreNormalOperation(void)
{
	dbw_.revMatchControl = FALSE;
	dbw_.revMatchTarget = NULL;
}
#endif

/**
 * @brief Set DBW throttle target.
 * 
 * If rev match control is enabled, and the rev match target is not null, and the target is in range,
 * then set the throttle target to the rev match target, otherwise set the target to the converted apps raw
 * value.
 * 
 * @return The throttle target value.
 */
static inline void DBW_SetTarget(void)
{
#if CONFIG_ENABLE_THROTTLE_BLIP
	/* Check if Rev match control is enabled */
	if ((dbw_.revMatchControl) &&
		NULL_CHECK1(dbw_.revMatchTarget) &&
		(DBW_IsRevMatchTargetInRange(*dbw_.revMatchTarget))) {
		apps_.target = *dbw_.revMatchTarget;
	} else {
		DBW_SetNormalApps();
	}
#else
	/* Normal target from APPS */
	DBW_SetNormalApps();
#endif
	apps_.target = CLAMP_MIN(apps_.target, TPS_IDLE);
}

static inline void DBW_UpdateAppsTps(void)
{
	/* Determine TPS position */
	tps_.position = DBWUtils_ConvertTpsRaw(dbw_.tps);
	DBWUtils_FilterSensor(&tps_.position, &tps_.kalman, &tps_.rcFilter, &apps_.iirFilter);

	/* Set target from APPS or Throttle Blip */
	DBW_SetTarget();

	dbw_.errPosPercent = ((apps_.target - tps_.position) / 1000.0f) * 100.0f; // 0-100 [%]

#if CONFIG_ADC_SHOW_MIN_MAX
	static bool_t startPosMeas = FALSE;
	if (tps_.position > TPS_F_MIN_CALC_THRESHOLD) {
		startPosMeas = TRUE;
	}

	if (startPosMeas) {
		Utils_UpdateMinMax_F(tps_.position, &tps_.posMin, &tps_.posMax);
	}
#endif
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
 * @brief Drive-By-Wire main state machine.
 */
static void DBW_StateMachine(void)
{
	/* Update APPS & TPS sensor values */
	DBW_UpdateAppsTps();

	if (dbw_.state != DBW_DISABLED &&
		dbw_.state != DBW_DISABLED_UNRECOVERABLE) {
		if (dbw_.apps_calib_request == TRUE) {
			dbw_.state = DBW_CALIBRATE_APPS;
			dbw_.apps_calib_request = FALSE;
		}

#if CONFIG_MAP_RPM_TEST
	if (DBWTest_GetState() != DBW_TEST_IDLE) {
		dbw_.apps->target = DBWTest_GetTpsTarget();
	}
#endif
	}

	switch (dbw_.state) {
		case DBW_INIT:
			dbw_.state = DBW_HandlerInit();
			break;

		case DBW_CALIBRATE_APPS:
			dbw_.state = DBWCalibration_APPS(dbw_.apps);
			break;

		case DBW_CALIBRATE_TPS:
			dbw_.state = DBWCalibration_TPS(dbw_.tps);
			break;

		case DBW_RUN:
			DBWSafety_Plausibility(dbw_.tps, dbw_.apps);

			/* NORMAL OPERATION */
			(void)DBWSafety_CheckPowerOff(&dbw_);

			/* Safety Check might disable DBW */
			if (dbw_.state == DBW_RUN) {
				dbw_.state = DBW_HandlerRun();
			}
			break;

		case DBW_DISABLED:
			DBWSafety_Plausibility(dbw_.tps, dbw_.apps);

			if (DBWSafety_CheckPowerOff(&dbw_) == SAFETY_POWER_OFF_RECOVER) {
				/* Go back to normal operation */
				DCMotor_Enable();
				LED_SetStatus(LED_SOLID);
				dbw_.state = DBW_HandlerRun();
			} else {
				/* Recovery in progress/fail still happening */
				DCMotor_Disable();
				LED_SetStatus(LED_BLINK_1HZ);
			}
			break;

		case DBW_DISABLED_UNRECOVERABLE:
		default:
			/* Unrecoverable */
			TurnOffSafetyLine(&dbw_);
			DCMotor_Disable();
			LED_SetStatus(LED_BLINK_1HZ);
			break;
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

	tps_.tps1 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_TPS_2);
	tps_.tps2 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_TPS_1);
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

#if CONFIG_MAP_RPM_TEST
		if (err == ERROR_OK) {
			err = DBWTest_Init();
		}
#endif

		err = DBWSafety_Init(&dbw_);

		if (err == ERROR_OK) {
			if (DCMotor_Init() == ERROR_OK) {
				dbw_.state = DBW_INIT;

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
 * @brief Main process function that is called from the Scheduler.
 */
void DBW_Process(void)
{
	if (DCMotor_GetState() == DC_MOTOR_FAILURE) {
		dbw_.state = DBW_DISABLED_UNRECOVERABLE;
	}

	DBW_StateMachine();

#if CONFIG_MAP_RPM_TEST
	DBWTest_Process();
#endif
}

/**
 * @brief Request APPS sensor calibration.
 * 
 * It starts a timer, sets the calibration limits to the maximum and minimum possible values, and sets
 * the state to calibrate the APPS.
 */
void DBW_RequestAppsCalibration(void)
{
	if ((dbw_.state != DBW_DISABLED) &&
		(dbw_.state != DBW_DISABLED_UNRECOVERABLE)) {
		DCMotor_Disable();
		SwTimerStart(&apps_.timer, APPS_CALIBRATION_TIME_MS);
		apps_.limits->calibMin = UINT16_MAX;
		apps_.limits->calibMax = 0U;
		dbw_.apps_calib_request = TRUE;
	}
}


#else
ErrorEnum DBW_Init(void) { return ERROR_OK; }
void DBW_Process(void) {}
void DBW_RequestAppsCalibration(void) {}
void DBW_Disable(boolean isError) { (void)isError; }
#endif

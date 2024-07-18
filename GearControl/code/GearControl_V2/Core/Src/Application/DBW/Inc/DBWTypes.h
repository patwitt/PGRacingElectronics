/*
 * DBWTypes.h
 *
 *  Created on: 7 lip 2024
 *      Author: lorem
 */

#ifndef SRC_APPLICATION_DBW_SRC_DBWTYPES_H_
#define SRC_APPLICATION_DBW_SRC_DBWTYPES_H_

#include "Types.h"
#include "DefineConfig.h"
#include "stm32f4xx_hal.h"
#include "Adc.h"
#include "DCMotor.h"
#include "Utils.h"

#include "SwTimer.h"
#include "RCFilter.h"
#include "IIRFilter.h"
#include "KalmanFilter.h"

#define TPS_BIAS_OFFSET (14.0f)
#define TPS_IDLE (10.0f + TPS_BIAS_OFFSET) // 6%

#define APPS_CALIBRATION_TIME_MS (5000U)
#define TPS_CALIBRATION_TIME_MS (1000U)

#define ADC_MAX (4096U)
#define TPS_ADC_MAX_DIFF_THRESHOLD (410U)  // 10% of 5V
#define APPS_ADC_MAX_DIFF_THRESHOLD (410U) // 10% of 5V

#define TPS_DEBOUNCE_MS (100U)
#define APPS_DEBOUNCE_MS (100U)

#define TPS_INIT_DELAY_MS (100U)
#define TPS_INIT_CALIBRATION_MS (500U)

/* CALIBRATION VALUES START */
/* MEASURED LOWER THRESHOLD MUST BE LOWER THAN FEASIBLE MIN! */
#define APPS_FEASIBLE_MIN  (1250U)
/* MEASURED HIGHER THRESHOLD MUST BE HIGHER THAN FEASIBLE MAX! */
#define APPS_FEASIBLE_MAX  (2600U)
#define APPS_MIN_MEASURED_F (1350.0f)
#define APPS_MAX_MEASURED_F (2450.0f)

#define TPS_FEASIBLE_MIN (250U)
#define TPS_FEASIBLE_MAX (4000U)
#define TPS_MIN_MEASURED_F (900.0f)
#define TPS_MAX_MEASURED_F (3900.0f)
/* CALIBRATION VALUES END */

#define APPS_POS_MAX_F (1000.0f)
#define APPS_POS_MIN_F (0.0f)
#define APPS_DIVISOR_F(min, max) (APPS_POS_MAX_F / (max - min))

#define TPS_F_MIN_CALC_THRESHOLD (500.0f)
#define TPS_CALIBRATION_OOR (4000U)
#define TPS_IDLE_POS_MAX_DIFF (20U)
#define TPS_MIN_CALIBRATION_PLAUSIBILITY_SAMPLES (50U)

#define TPS_CALIBRATION_SPEED (380.0f) // 0-1000
#define TPS_POS_MAX_F (1000.0f)
#define TPS_POS_MIN_F (0.0f)
#define TPS_DIVISOR_F(min, max) (TPS_POS_MAX_F / (max - min))

#define DBW_PWM_SPRING_MIN (300.0f)

/* Filter constants */
#define LPF_RC_CUTOFF_FREQ_HZ (50.0f)
#define LPF_RC_TIME_CONSTANT_S (0.001f)
#define IIR_FILTER_ALPHA (0.5f)

#define SAFETY_MAX_TARGET_TO_POSITION_DIFF (101.0f)
#define SAFETY_POWER_OFF_RECOVER (TRUE)

#define ENGINE_RPM_MIN (500U)
#define ENGINE_RPM_MAX (14000U)

typedef struct {
	GPIO_TypeDef *const gpioPort;
	const uint16_t gpioPin;
	uint32_t safetyCnt;
	boolean status;
} DBWSafety_TriggerHandler;

typedef struct {
	const uint16 maxDiffAllowed;
	const uint8_t debounceMs;
	const ErrorEnum errorFlag;
	uint16_t absDiff;
	uint16_t maxAbsDiff;
	uint8_t debounceCnt;
} DBWSafety_PlausibilityType;

typedef struct {
	float max;
	float min;
	uint16 calibMin;
	uint16 calibMax;
	const uint16 feasibleMin;
	const uint16 feasibleMax;
} SensorLimitsType;

typedef enum
{
	DBW_INIT,
	DBW_RUN,
	DBW_CALIBRATE_APPS,
	DBW_CALIBRATE_TPS,
#if CONFIG_MAP_RPM_TEST
	DBW_RPM_TEST,
#endif
	DBW_DISABLED,
	DBW_DISABLED_UNRECOVERABLE
} DBW_States;

typedef enum {
	REV_MATCH_DBW_FAILURE,
	REV_MATCH_TARGET_INVALID,
	REV_MATCH_DBW_OK
} DbwRevMatchStatus;

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
	DBWSafety_PlausibilityType *const plausibility;
	SensorLimitsType *const limits;
	ErrorEnum error;
	SwTimerType timer;
	uint16 calibOkCnt;
	uint16 calibNokCnt;
	RCFilter rcFilter;
	IIRFilter iirFilter;
	float posMin;
	float posMax;
	float percent;
	KalmanFilter kalman;
} TpsSensorType;

typedef struct {
	/* APPS1 ADC Channel */
	__IO AdcDataChannel* apps1;
	/* APPS2 ADC Channel */
	__IO AdcDataChannel* apps2;
	ErrorEnum error;
	DBWSafety_PlausibilityType *const plausibility;
	SensorLimitsType *const limits;
	float target; /* Target position 0-1000 -> 0.1% of max range (around 50deg) */
	float interpolatedTarget; /* Interpolated target position */
	SwTimerType timer;
	RCFilter rcFilter;
	IIRFilter iirFilter;
	float posMin;
	float posMax;
	float percent;
	KalmanFilter kalman;
} AppsSensorType;

typedef struct
{
	float errPosPercent;
	TpsSensorType *const tps;
	AppsSensorType *const apps;
	/* DBW state */
	DBW_States state;
	DBWSafety_TriggerHandler *const safetyTrigger;
	bool_t apps_calib_request;
#if CONFIG_ENABLE_THROTTLE_BLIP
	bool_t revMatchControl;
	float* revMatchTarget;
#endif
} DbwHandle;


#endif /* SRC_APPLICATION_DBW_SRC_DBWTYPES_H_ */

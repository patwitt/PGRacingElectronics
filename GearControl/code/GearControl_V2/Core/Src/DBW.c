/*
 * DBW.c
 *
 *  Created on: 22.05.2021
 *      Author: Patryk Wittbrodt
 */

#include "GearControl.h"
#include "GearSensor.h"
#include "main.h"
#include "stdlib.h"
#include "Adc.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
extern TIM_HandleTypeDef htim1;

/*
 * CLOSED
 * TPS1 - 3522
 * TPS2 - 730-732
 *
 * OPENED
 * TPS 1 - 730
 * TPS 2 - 4070
 */

#define TPS_ADC_MAX (4096U)
#define APPS_ADC_MAX (4096U)
#define TPS_ADC_MAX_DIFF_THRESHOLD (200U)
#define TPS_DEBOUNCE_MS (100U)
#define APPS_DEBOUNCE_MS (100U)

#define TPS_INIT_DELAY_MS (100U)
#define TPS_INIT_CALIBRATION_MS (1000U)
#define TPS_INIT_IDLE_EXPECTED (733U)

typedef enum
{
	DBW_INIT,
	DBW_RUN,
	DBW_DISABLED
} DBW_States;

typedef struct {
	const uint16 maxAdc;
	const uint16 maxDiffAllowed;
	const uint8_t debounceMs;
	const ErrorFlagsEnum errorFlag;
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
	/* Throttle Position 0-100 */
	uint8_t posPercent;
	PlausibilityParamType *const plausibility;
	ErrorFlagsEnum error;
} TpsSensorType;

typedef struct {
	/* Valid status */

	/* APPS1 ADC Channel */
	__IO AdcDataChannel* apps1;
	/* APPS2 ADC Channel */
	__IO AdcDataChannel* apps2;
	ErrorFlagsEnum error;
	PlausibilityParamType *const plausibility;
} AppsSensorType;

typedef struct {
	__IO uint32* PWM_H_plus;
	__IO uint32* PWM_H_minus;
} EtcController;

typedef struct
{
	TpsSensorType *const tps;
	AppsSensorType *const apps;
	/* DBW state */
	DBW_States state;
} DbwHandle;

static PlausibilityParamType tpsPlausibility = {.maxAbsDiff = 0U, .maxAdc = TPS_ADC_MAX, .maxDiffAllowed = TPS_ADC_MAX_DIFF_THRESHOLD, .debounceMs = TPS_DEBOUNCE_MS, .errorFlag = ERROR_DBW_TPS_PLAUSIBILITY};
static PlausibilityParamType appsPlausibility = {.maxAbsDiff = 0U,.maxAdc = APPS_ADC_MAX, .maxDiffAllowed = TPS_ADC_MAX_DIFF_THRESHOLD, .debounceMs = APPS_DEBOUNCE_MS, .errorFlag = ERROR_DBW_APPS_PLAUSIBILITY};

/* Sensors */
static TpsSensorType tps_ = {.tps1 = NULL, .tps2 = NULL, .idlePosMin = 0xFFFFU, .idlePosMax = 0U, .error = ERROR_OK, .plausibility = &tpsPlausibility};
static AppsSensorType apps_ = {.apps1 = NULL, .apps2 = NULL, .error = ERROR_OK, .plausibility = &appsPlausibility};

/* ETC */
static EtcController etc = {.PWM_H_minus = NULL, .PWM_H_plus = NULL};

/* DBW */
static DbwHandle dbw = {.tps = &tps_, .apps = &apps_, .state = DBW_DISABLED};

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */
void DBW_Disable(void);
static DBW_States DBW_Handler_Init(void);
static DBW_States DBW_Handler_Run(void);

static void DBW_GetCurrentTPSDeg(void);
static void DBW_PlausibilityCheck(PlausibilityParamType *const plausibility, const uint16 sens1, const uint16 sens2, ErrorFlagsEnum *const error);

ErrorFlagsEnum DBW_Init(void)
{
	ErrorFlagsEnum err = ERROR_OK;

	tps_.tps1 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_TPS_1);
	tps_.tps2 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_TPS_2);
	apps_.apps1 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_APPS_1);
	apps_.apps2 = ADC_getAdcChannelPtr(ADC_2_HANDLE, ADC_CHANNEL_APPS_2);

	if ((tps_.tps1 != NULL) && (tps_.tps1->rawVal != NULL) &&
		(tps_.tps2 != NULL) && (tps_.tps2->rawVal != NULL) &&
		(apps_.apps1 != NULL) && (apps_.apps1->rawVal != NULL) &&
		(apps_.apps2 != NULL) && (apps_.apps2->rawVal != NULL)) {

		/* Enable ETC */
		HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(INH1_GPIO_Port, INH1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(INH2_GPIO_Port, INH2_Pin, GPIO_PIN_SET);

		/* Start PWM timers for ETC */
		if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK) {
		  err = ERROR_HAL;
		}

		if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4) != HAL_OK) {
		  err = ERROR_HAL;
		}

		if (err == ERROR_OK) {
		  htim1.Instance->CCR1 = 0;
		  htim1.Instance->CCR4 = 0;
		  etc.PWM_H_plus = &htim1.Instance->CCR1;
		  etc.PWM_H_minus = &htim1.Instance->CCR4;
		  dbw.state = DBW_INIT;
		}
	} else {
		err = ERROR_NULL;
	}

	return err;
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
					nextState = DBW_RUN;
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
	static uint32 ticks = 0;

	if (ticks < 500) {
		if (*etc.PWM_H_minus == 0) {
			*etc.PWM_H_plus = 2500;
		}
		*etc.PWM_H_minus = 0;
	} else {
		if (*etc.PWM_H_plus == 0) {
			*etc.PWM_H_minus = 2500;
		}
		*etc.PWM_H_plus = 0;
	}

	++ticks;
	if (ticks > 1000) {
		ticks = 0U;
	}

	return DBW_RUN;
}

void DBW_Disable(void)
{
	/* Disable DBW */
	dbw.state = DBW_DISABLED;
	*etc.PWM_H_plus = 0;
	*etc.PWM_H_minus = 0;
	HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(INH1_GPIO_Port, INH1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(INH2_GPIO_Port, INH2_Pin, GPIO_PIN_RESET);
}

void DBW_Process(void)
{
	switch (dbw.state)
	{
		case DBW_INIT:
			dbw.state = DBW_Handler_Init();
			break;

		case DBW_RUN:
			DBW_PlausibilityCheck(tps_.plausibility, tps_.tps1->avg, tps_.tps2->avg, &tps_.error);
			DBW_PlausibilityCheck(apps_.plausibility, apps_.apps1->avg, apps_.apps2->avg, &apps_.error);

			if ((apps_.error == ERROR_OK) && (tps_.error == ERROR_OK)) {
				dbw.state = DBW_Handler_Run();
			} else {
				dbw.state = DBW_DISABLED;
			}
			break;

		case DBW_DISABLED:
		default:
		    DBW_Disable();
			break;
	}
}

uint16 DBW_GetCurrentTPSRaw(void)
{
	//return dbw.adcTps2->avg;
	return 0;
}

static void DBW_GetCurrentTPSDeg(void)
{
	//dbw.throttlePos = abs(dbw.adcTps2->avg - dbw.constTpsIdle) / 28U;
}

static void DBW_PlausibilityCheck(PlausibilityParamType *const plausibility, const uint16 sens1, const uint16 sens2, ErrorFlagsEnum *const error)
{
	plausibility->absDiff = (uint16_t)abs(plausibility->maxAdc - (sens1 + sens2));

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

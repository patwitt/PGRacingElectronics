/*
 * GearSensor.c
 *
 *  Created on: 22.05.2021
 *      Author: Patryk Wittbrodt
 */

#include "GearSensor.h"
#include "Adc.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

#define GEAR_SENS_ADC_UNKNOWN (0U)

typedef enum {
	GEAR_SENS_STATUS_NOT_IN_RANGE,
	GEAR_SENS_STATUS_IN_RANGE,
	GEAR_SENS_STATUS_CHECK_IN_PROGRESS
} GearSensorStatusEnum;

typedef struct {
	const GearSensorStatesEnum state;
	const uint16 adcExpectedReading;
	const uint16 adcResolution;
	uint32_t validCnt;
} GearSensorConfigType;

static GearSensorConfigType GearSensorConfig[GEAR_SENS_COUNT] = {
	 [GEAR_SENS_1] = {
		 .adcExpectedReading = 2690U ,.adcResolution = 80U, .validCnt = 0U
	 },
	 [GEAR_SENS_N] = {
		 .adcExpectedReading = 4000U ,.adcResolution = 200U, .validCnt = 0U
	 },
	 [GEAR_SENS_2] = {
	     .adcExpectedReading = 1938U ,.adcResolution = 80U, .validCnt = 0U
	 },
	 [GEAR_SENS_3] = {
		 .adcExpectedReading = 1390U ,.adcResolution = 80U, .validCnt = 0U
	 },
	 [GEAR_SENS_4] = {
	     .adcExpectedReading = 888U ,.adcResolution = 80U, .validCnt = 0U
	 },
	 [GEAR_SENS_5] = {
		 .adcExpectedReading = 518U ,.adcResolution = 80U, .validCnt = 0U
	 },
	 [GEAR_SENS_6] = {
	     .adcExpectedReading = 190U ,.adcResolution = 80U, .validCnt = 0U
	 },
	 [GEAR_SENS_IN_PROG] = {
		 .adcExpectedReading = 0xFFFFU ,.adcResolution = 0U, .validCnt = 0U
	 },
	 [GEAR_SENS_INVALID] = {
		 .adcExpectedReading = 0U ,.adcResolution = 0U, .validCnt = 0U
	 }
};

typedef struct {
	/* Gear Sensor Channel */
	__IO AdcDataChannel* gearSensAdc;
	GearSensorConfigType *const config;
	GearSensorStatesEnum state;
	uint32_t validCnt;
	const uint32_t nChecks;
} GearSensorHandler;

GearSensorHandler gearSens = {.gearSensAdc = NULL, .state = GEAR_SENS_INVALID, .config = GearSensorConfig, .validCnt = 0U, .nChecks = 3U};

GearSensorStatesEnum GearSensor_GetState(void)
{
	return gearSens.state;
}

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */
static inline GearSensorStatusEnum GearSensor_ValidateRange(const GearSensorStatesEnum testedGear);
static inline bool_t GearSensor_IsInRange(const uint16_t adcReading,
		                                  const uint16_t resolution,
										  const uint16_t expected);
/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static inline bool_t GearSensor_IsInRange(const uint16_t adcReading, const uint16_t resolution, const uint16_t expected) {
	return ((adcReading > GEAR_SENS_ADC_UNKNOWN) &&
			(expected >= (*gearSens.gearSensAdc->raw - resolution)) &&
			(expected <= (*gearSens.gearSensAdc->raw + resolution)));
}

/* Check if adc reading is within threshold for given gear position */
static inline GearSensorStatusEnum GearSensor_ValidateRange(const GearSensorStatesEnum testedGear)
{
	GearSensorStatusEnum inRange = GEAR_SENS_STATUS_NOT_IN_RANGE;
	GearSensorConfigType *const testedGearCfg = &gearSens.config[testedGear];

	if (GearSensor_IsInRange(*gearSens.gearSensAdc->raw, testedGearCfg->adcResolution, testedGearCfg->adcExpectedReading)) {
		if (testedGearCfg->validCnt >= gearSens.nChecks) {
			inRange = GEAR_SENS_STATUS_IN_RANGE;
			testedGearCfg->validCnt = 0U;
		} else {
			inRange = GEAR_SENS_STATUS_CHECK_IN_PROGRESS;
			++testedGearCfg->validCnt;
		}
	} else {
		/* Reset valid counter and return NOT IN RANGE to test the next one */
		testedGearCfg->validCnt = 0U;
	}

	return inRange;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum GearSensor_Init(void)
{
	ErrorEnum error = ERROR_OK;

	gearSens.gearSensAdc = ADC_getAdcChannelPtr(ADC_1_HANDLE, ADC_CHANNEL_GEAR_SENS);

	if (gearSens.gearSensAdc != NULL) {
		/* Init OK */
	} else {
		error = ERROR_NULL;
	}

	return error;
}

/* Get current gear by sensor reading */
GearSensorStatesEnum GearSensor_GetStateBySensorAdc(void)
{
	GearSensorStatesEnum gearTestIndex  = GEAR_SENS_1; /* Start with GEAR 1 */
	GearSensorStatesEnum recognizedGear = GEAR_SENS_INVALID;

	while ((gearTestIndex != GEAR_SENS_INVALID) && (recognizedGear == GEAR_SENS_INVALID)) {
		const GearSensorStatusEnum isInRangeStatus = GearSensor_ValidateRange(gearTestIndex);

		switch (isInRangeStatus) {
			case GEAR_SENS_STATUS_IN_RANGE:
				/* Gear found, return recognized gear */
				recognizedGear = gearTestIndex;
				break;

			case GEAR_SENS_STATUS_CHECK_IN_PROGRESS:
				recognizedGear = GEAR_SENS_IN_PROG;
				break;

			case GEAR_SENS_STATUS_NOT_IN_RANGE:
			default:
				++gearTestIndex;
				break;
		}
	}

	return recognizedGear;
}

void GearSensor_Process(void)
{
	gearSens.state = GearSensor_GetStateBySensorAdc();
}


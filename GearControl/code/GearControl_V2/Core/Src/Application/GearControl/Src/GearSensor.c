/*
 * GearSensor.c
 *
 *  Created on: 06.07.2022
 *      Author: Patryk Wittbrodt
 */

#include "GearSensor.h"
#include "Adc.h"
#include "Utils.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

#define GEAR_SENS_ADC_READING_UNKNOWN_THRESHOLD (10U)

typedef enum {
	GEAR_SENS_STATUS_NOT_IN_RANGE,
	GEAR_SENS_STATUS_IN_RANGE,
	GEAR_SENS_STATUS_CHECK_IN_PROGRESS
} GearSensorStatusEnum;

//! Gear Sensor configuration struct
typedef struct {
	const GearSensorStatesEnum state;
	const uint16 adcExpectedReading;
	const uint16 adcResolution;
	uint32_t validCnt;
} GearSensorConfigType;

//! Gear Sensor handler struct
typedef struct {
	/* Gear Sensor Channel */
	__IO AdcDataChannel* gearSensAdc;
	GearSensorConfigType *const config;
	GearSensorStatesEnum state;
	const GearSensorPlausibilityEnum *const plausibilityMap;
	GearSensorPlausibilityEnum plausibility;
	uint32_t validCnt;
	uint32_t plausibilityCnt;
	uint32_t unknownReadingCnt;
	const uint32_t plausibilityDebounceMs;
	const uint32_t unknownDebounceMs;
	const uint32_t nChecks;
} GearSensorHandler;

//! Mapping of sensor debouncing <- plausibility
static const GearSensorPlausibilityEnum plausibilityDebounceMap[DEBOUNCE_STATUS_CNT + 1U] = {
	[DEBOUNCE_EXCEEDED]    = GEAR_SENS_IMPLAUSIBLE,
	[DEBOUNCE_CNT_ZERO]    = GEAR_SENS_PLAUSIBLE,
	[DEBOUNCE_IN_PROGRESS] = GEAR_SENS_PLAUSIBILITY_IN_PROGRESS,
	[DEBOUNCE_STATUS_CNT]  = GEAR_SENS_PLAUSIBILITY_CNT
};

//! Gear Sensor configuration
static GearSensorConfigType GearSensorConfig[GEAR_SENS_COUNT] = {
	 [GEAR_SENS_1] = {
		 .adcExpectedReading = 3350U,
		 .adcResolution = 80U,
		 .validCnt = 0U
	 },
	 [GEAR_SENS_N] = {
		 .adcExpectedReading = 4000U,
		 .adcResolution = 200U,
		 .validCnt = 0U
	 },
	 [GEAR_SENS_2] = {
	     .adcExpectedReading = 2780U,
		 .adcResolution = 80U,
		 .validCnt = 0U
	 },
	 [GEAR_SENS_3] = {
		 .adcExpectedReading = 2240U,
		 .adcResolution = 80U,
		 .validCnt = 0U
	 },
	 [GEAR_SENS_4] = {
	     .adcExpectedReading = 1640U,
		 .adcResolution = 80U,
		 .validCnt = 0U
	 },
	 [GEAR_SENS_5] = {
		 .adcExpectedReading = 1100U,
		 .adcResolution = 50U,
		 .validCnt = 0U
	 },
	 [GEAR_SENS_6] = {
	     .adcExpectedReading = 520U,
		 .adcResolution = 50U,
		 .validCnt = 0U
	 },
	 [GEAR_SENS_IN_PROG] = {
		 .adcExpectedReading = 0xFFFFU,
		 .adcResolution = 0U,
		 .validCnt = 0U
	 },
	 [GEAR_SENS_INVALID] = {
		 .adcExpectedReading = 0U,
		 .adcResolution = 0U,
		 .validCnt = 0U
	 }
};

//! Gear Sensor handler
static GearSensorHandler gearSens = {
		.gearSensAdc = NULL,
		.state = GEAR_SENS_INVALID,
	    .config = GearSensorConfig,
	    .validCnt = 0U,
        .plausibilityCnt = 0U,
		.unknownReadingCnt = 0U,
		.plausibility = GEAR_SENS_PLAUSIBILITY_IN_PROGRESS,
	    .nChecks = 3U,
	    .plausibilityDebounceMs = 20U,
		.unknownDebounceMs = 5U,
		.plausibilityMap = plausibilityDebounceMap
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline GearSensorStatesEnum GearSensor_GetStateBySensorAdc(void);
static inline void GearSensor_PlausibilityCheck(GearSensorStatesEnum *const gearSensReading);
static inline GearSensorStatusEnum GearSensor_ValidateRange(const GearSensorStatesEnum testedGear);
static inline bool_t GearSensor_IsInRange(const uint16_t adcReading,
		                                  const uint16_t resolution,
										  const uint16_t expected);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */
/**
 * @brief Check if ADC reading is in gear sensor range.
 *
 * If the ADC reading is greater than the unknown threshold, and the expected value is within the
 * resolution of the ADC reading, then return true.
 * 
 * @param adcReading The ADC reading from the gear sensor.
 * @param resolution The resolution of the ADC.
 * @param expected   the expected ADC reading for the gear sensor.
 * 
 * @return TRUE if reading is in range, FALSE otherwise.
 */
static inline bool_t GearSensor_IsInRange(const uint16_t adcReading, const uint16_t resolution, const uint16_t expected) {
	return ((adcReading >= GEAR_SENS_ADC_READING_UNKNOWN_THRESHOLD) &&
			(expected >= (*gearSens.gearSensAdc->raw - resolution)) &&
			(expected <= (*gearSens.gearSensAdc->raw + resolution)));
}

/**
 * @brief Validate range of potential gear.
 * 
 * If the ADC reading is in range, increment the valid counter and return
 * `GEAR_SENS_STATUS_CHECK_IN_PROGRESS` if the valid counter is less than the number of checks
 * required, otherwise return `GEAR_SENS_STATUS_IN_RANGE` and reset the valid counter. Otherwise, reset
 * the valid counter and return `GEAR_SENS_STATUS_NOT_IN_RANGE`
 * 
 * @param testedGear The gear we're testing.
 * 
 * @return Gear sensor status.
 */
static inline GearSensorStatusEnum GearSensor_ValidateRange(const GearSensorStatesEnum testedGear)
{
	GearSensorStatusEnum inRange = GEAR_SENS_STATUS_NOT_IN_RANGE;
	GearSensorConfigType *const testedGearCfg = &gearSens.config[testedGear];

	if (GearSensor_IsInRange(*gearSens.gearSensAdc->raw,
			                 testedGearCfg->adcResolution,
							 testedGearCfg->adcExpectedReading)) {
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

/**
 * @brief Process gear sensor by ADC readings.
 * 
 * It checks if the ADC value is within the range of a gear, and if it is, it returns the gear.
 * 
 * @return The gear that is read by sensor.
 */
static inline GearSensorStatesEnum GearSensor_ProcessSensorAdc(void)
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
				/* Check in progress for potential gear */
				recognizedGear = GEAR_SENS_IN_PROG;
				break;

			case GEAR_SENS_STATUS_NOT_IN_RANGE:
			default:
				/* For this gear read is not in range,
				 * proceed check for the next ones */
				++gearTestIndex;
				break;
		}
	}

	return recognizedGear;
}

/**
 * @brief First layer function to get gear by sensor reading.
 * 
 * If the ADC reading is below a certain threshold, then the gear sensor is in an unknown state.
 * Otherwise, the gear sensor is in a known state and gear will be processed by ADC reading.
 * 
 * @return Recognized gear by the gear sensor.
 */
static inline GearSensorStatesEnum GearSensor_GetStateBySensorAdc(void)
{
	GearSensorStatesEnum recognizedGear = GEAR_SENS_INVALID;

	const bool_t unknownGearCondition = (*gearSens.gearSensAdc->raw < GEAR_SENS_ADC_READING_UNKNOWN_THRESHOLD);

	const UtilsDebounceStatus unknownGearDebounce = Utils_Debounce(unknownGearCondition,
			                                                       &gearSens.unknownReadingCnt,
											                       gearSens.unknownDebounceMs);

	switch (unknownGearDebounce) {

		case DEBOUNCE_EXCEEDED:
		case DEBOUNCE_IN_PROGRESS:
			/* Unknown gear - gear sensor positioned between gears */
			recognizedGear = GEAR_SENS_UNKNOWN;
			break;

		case DEBOUNCE_CNT_ZERO:
			/* Normal operation */
			recognizedGear = GearSensor_ProcessSensorAdc();
			break;

		case DEBOUNCE_STATUS_CNT:
		default:
			/* OOR */
			break;
	}

	return recognizedGear;
}

/**
 * @brief Gear sensor plausibility check and debouncing mechanism.
 * 
 * @param gearSensReading The current gear sensor reading.
 */
static inline void GearSensor_PlausibilityCheck(GearSensorStatesEnum *const gearSensReading)
{
	/* Debouncing of Gear sensor reading */
	const bool_t gearSensFailCondition = (*gearSensReading > GEAR_SENS_6);

	const UtilsDebounceStatus debounce = Utils_Debounce(gearSensFailCondition,
			                                            &gearSens.plausibilityCnt,
											            gearSens.plausibilityDebounceMs);

	gearSens.plausibility = gearSens.plausibilityMap[debounce];
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
/**
 * @brief Initialization of Gear Sensor module.
 * 
 * The function initializes the gear sensor by getting a pointer to the ADC channel that the gear
 * sensor is connected to.
 * 
 * @return an error code.
 */
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

/**
 * @brief Get the current state of the gear sensor.
 * 
 * @return The state of the gear sensor.
 */
GearSensorStatesEnum GearSensor_GetState(void)
{
	return gearSens.state;
}

/**
 * @brief Get the plausibility of the gear sensor.
 * 
 * @return The plausibility of the gear sensor.
 */
GearSensorPlausibilityEnum GearSensor_GetPlausibility(void)
{
	return gearSens.plausibility;
}

/**
 * @brief Main process function that is called from the Scheduler.
 */
void GearSensor_Process(void)
{
	const GearSensorStatesEnum sensorReading = GearSensor_GetStateBySensorAdc();

	if (sensorReading != GEAR_SENS_IN_PROG) {
		gearSens.state = sensorReading;
		/* Check plausibility if not unknown gear and not in progress */
		if (sensorReading != GEAR_SENS_UNKNOWN) {
			GearSensor_PlausibilityCheck(&gearSens.state);
		}
	}
}

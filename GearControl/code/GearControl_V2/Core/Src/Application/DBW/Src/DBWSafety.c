/*
 * DBW_Safety.c
 *
 *  Created on: 7 lip 2024
 *      Author: Patryk Wittbrodt
 */

#include "DBWSafety.h"
#include "DBWUtils.h"
#include "stdlib.h"
#include "SwTimer.h"
/* Plausibility */
#if CONFIG_DBW_ADJUST_SENS_LIMITS
static void DBW_AdjustSensorLimits(SensorLimitsType *const limits, const uint16 invalidValue);
#endif

typedef enum  {
	TRIGGER_POLL,      // CV1.6.9
	TRIGGER_POWER_OFF, // CV1.6.5
	POWER_OFF_RECOVER  // CV1.6.5
} TriggerStates;

#define SAFETY_TRIGGER_MS (500U)
#define SAFETY_POWER_OFF_MS (1000U)
#define SAFETY_POWER_OFF_RESET_MS (1000U)

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */

static inline void TurnOffSafetyLine(DbwHandle *const dbw_)
{
	HAL_GPIO_WritePin(dbw_->safetyTrigger->gpioPort, dbw_->safetyTrigger->gpioPin, GPIO_PIN_RESET);
	dbw_->safetyTrigger->status = false;
}

static inline void TurnOnSafetyLine(DbwHandle *const dbw_)
{
	HAL_GPIO_WritePin(dbw_->safetyTrigger->gpioPort, dbw_->safetyTrigger->gpioPin, GPIO_PIN_SET);
	dbw_->safetyTrigger->status = true;
}

static inline TriggerStates ProcessTriggerPoll(DbwHandle *const dbw_, const bool_t safetyOK)
{
	TriggerStates state = TRIGGER_POLL;

	if (safetyOK) {
		dbw_->safetyTrigger->safetyCnt = 0U;
	} else {
		if (dbw_->safetyTrigger->safetyCnt <= SAFETY_TRIGGER_MS) {
			++dbw_->safetyTrigger->safetyCnt;
		}
	}

	/* CHECK ALL TRIGGERS - PLAUSIBILITY & TPS -> APPS ERROR */
	if ((dbw_->safetyTrigger->safetyCnt > SAFETY_TRIGGER_MS) ||
		(dbw_->apps->error != ERROR_OK) ||
		(dbw_->tps->error != ERROR_OK)) {
		/* Disable DBW and go to CV1.6.5 Check */
		DBW_Disable(dbw_);
		state = TRIGGER_POWER_OFF;
		/* Reset counter */
		dbw_->safetyTrigger->safetyCnt = 0U;
		/* Reset sensor errors */
		dbw_->apps->error = ERROR_OK;
		dbw_->tps->error = ERROR_OK;
	}

	return state;
}

static inline TriggerStates ProcessPowerOff(DbwHandle *const dbw_)
{
	TriggerStates state = TRIGGER_POWER_OFF;

	if (DBWUtils_IsIdle(dbw_->tps)) {
		state = POWER_OFF_RECOVER;
		dbw_->safetyTrigger->safetyCnt = 0U;
	} else {
		++dbw_->safetyTrigger->safetyCnt;
		// N ms elapsed, if TPS is NOT IDLE +/- 5%, SAFETY LINE OFF!
		if (dbw_->safetyTrigger->safetyCnt > SAFETY_POWER_OFF_MS) {
			TurnOffSafetyLine(dbw_);
			state = POWER_OFF_RECOVER;
			dbw_->safetyTrigger->safetyCnt = 0U;
		}
	}

	return state;
}

static inline TriggerStates ProcessPowerOffRecover(DbwHandle *const dbw_, const bool_t safetyOK, bool_t *const recover)
{
	TriggerStates state = POWER_OFF_RECOVER;

	if ((safetyOK) && (DBWUtils_IsIdle(dbw_->tps))) {
		if (dbw_->safetyTrigger->safetyCnt < SAFETY_POWER_OFF_RESET_MS) {
			++dbw_->safetyTrigger->safetyCnt;
		}
	} else {
		dbw_->safetyTrigger->safetyCnt = 0U;
	}

	if (dbw_->safetyTrigger->safetyCnt > SAFETY_POWER_OFF_RESET_MS) {
		// Target is OK for at least N ms
		TurnOnSafetyLine(dbw_);
		*recover = SAFETY_POWER_OFF_RECOVER;
		dbw_->safetyTrigger->safetyCnt = 0U;
		state = TRIGGER_POLL;
	}

	return state;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

/**
 * @brief Drive-By-Wire Power to the line safety check.
 *
 * CV1.6.5
 *
 * When power is removed, the electronic throttle must immediately close at least
 * to idle position ±5%. An interval of one second is allowed for the throttle to
 * close to idle, failure to achieve this within the required interval must result
 * in immediate disabling of power to ignition, fuel injectors and fuel pump.
 * This action must remain active until the TPS signals indicate the throttle has
 * returned to idle position ±5% for at least one second.
 *
 * CV1.6.9
 *
 * The power to the electronic throttle must be immediately shut down,
 * as defined in CV1.6.5, if the throttle position differs by more than 10%
 * from the expected target TPS position for more than 500ms.
 *
 * @return TRUE if recover should happen, FALSE otherwise
 */
bool_t DBWSafety_CheckPowerOff(DbwHandle *const dbw_)
{
	bool_t recover = FALSE;

	static TriggerStates state = TRIGGER_POLL;

	bool_t safetyOK = (abs(dbw_->tps->position - dbw_->apps->target) <= SAFETY_MAX_TARGET_TO_POSITION_DIFF);

	switch (state) {
		case TRIGGER_POLL:
			state = ProcessTriggerPoll(dbw_, safetyOK);
			break;

		case TRIGGER_POWER_OFF:
			state = ProcessPowerOff(dbw_);
			break;

		case POWER_OFF_RECOVER:
		default:
			state = ProcessPowerOffRecover(dbw_, safetyOK, &recover);
			break;
	}

	return recover;
}

/**
 * @brief Drive-By-Wire sensors plausibility checks.
 *
 * CV1.6.6
 *
 * If plausibility does not occur between the values of at least two TPSs
 * and this persists for more than 100ms, the power to the electronic throttle
 * must be immediately shut down. Plausibility is defined as a deviation of
 * less than ten percentage points between the sensor values as defined in CV1.4.3
 * and no detected failures as defined in T11.9.
 * AS must check this signal consistency on a low level itself.
 */
void DBWSafety_Plausibility(DBWSafety_PlausibilityType *const plausibility,
								 SensorLimitsType *const limits,
								 const uint16 sens1, const uint16 sens2,
								 ErrorEnum *const error)
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
		} else if (plausibility->debounceCnt > plausibility->debounceMs) {
			*error = plausibility->errorFlag;
		} else { /* Nothing */ }
	}
}

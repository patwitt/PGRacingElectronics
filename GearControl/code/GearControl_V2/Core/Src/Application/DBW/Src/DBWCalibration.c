/*
 * DBWCalibration.c
 *
 *  Created on: 7 lip 2024
 *      Author: Patryk Wittbrodt
 */

#include "DBWCalibration.h"
#include "stdlib.h"
#include "SwTimer.h"
#include "DCMotor.h"
#include "LED.h"

DBW_States DBWCalibration_APPS(AppsSensorType *const apps_)
{
#define APPS_CALIB_MAX_LIMIT_PERCENT (1.0f)

	DBW_States nextState = DBW_CALIBRATE_APPS;
	apps_->plausibility->absDiff = (uint16_t)abs(ADC_MAX - (*apps_->apps1->raw + *apps_->apps2->raw));
	Utils_UpdateMax_U16(apps_->plausibility->absDiff, &apps_->plausibility->maxAbsDiff);

	if (!SwTimerHasElapsed((const SwTimerType*)&apps_->timer)) {
		if (apps_->plausibility->absDiff < apps_->plausibility->maxDiffAllowed) {
			/* Use 10 sample average OR apps2->raw */
			Utils_UpdateMinMax_U16(apps_->apps2->avgData.avg, &apps_->limits->calibMin, &apps_->limits->calibMax);
		}
	} else {
		if ((apps_->plausibility->absDiff < apps_->plausibility->maxDiffAllowed) &&
		    (apps_->limits->calibMin < APPS_FEASIBLE_MIN) &&
		    (apps_->limits->calibMax > APPS_FEASIBLE_MAX)) {
			/* Calibration OK */
			apps_->limits->min = (float)apps_->limits->calibMin;
			apps_->limits->max = (float)(apps_->limits->calibMax) * ((100.0f - APPS_CALIB_MAX_LIMIT_PERCENT) / 100.0f);
			DCMotor_Enable();
			nextState = DBW_RUN;
		} else {
			nextState = DBW_DISABLED_UNRECOVERABLE;
			apps_->error = ERROR_DBW_APPS_CALIBRATION;
		}
	}

	return nextState;
}

DBW_States DBWCalibration_TPS(TpsSensorType *const tps_)
{
	DBW_States nextState = DBW_CALIBRATE_TPS;

	tps_->plausibility->absDiff = (uint16_t)abs(ADC_MAX - (*tps_->tps2->raw + *tps_->tps1->raw));
	Utils_UpdateMax_U16(tps_->plausibility->absDiff, &tps_->plausibility->maxAbsDiff);

	if (!SwTimerHasElapsed((const SwTimerType*)&tps_->timer)) {
		/* Run TPS calibration to set new MIN/MAX ADC values */
		const bool_t isPlausible = (tps_->plausibility->absDiff < tps_->plausibility->maxDiffAllowed);

		if (isPlausible) {
			++tps_->calibOkCnt;
			/* Continue going UP/DOWN */
			/* Use 10 sample average OR tps2->raw */
			Utils_UpdateMinMax_U16(tps_->tps2->avgData.avg, &tps_->limits->calibMin, &tps_->limits->calibMax);
		} else {
			/* Invalid plausibility, reached end */
			if (tps_->calibNokCnt > tps_->calibOkCnt) {
				if (tps_->calibNokCnt > TPS_MIN_CALIBRATION_PLAUSIBILITY_SAMPLES) {
					nextState = DBW_DISABLED_UNRECOVERABLE;
					tps_->error = ERROR_DBW_TPS_CALIBRATION;
				}
			}
			++tps_->calibNokCnt;
		}

		switch (tps_->calibrationDirection) {
			case DC_MOTOR_ROTATE_PLUS:
				 if ((!isPlausible) && ((tps_->calibOkCnt > TPS_MIN_CALIBRATION_PLAUSIBILITY_SAMPLES) || (*tps_->tps2->raw > TPS_CALIBRATION_OOR))) {
						/* UP calibration finished successfully, change direction */
						tps_->calibNokCnt = 0U;
						tps_->calibOkCnt = 0U;
						tps_->calibrationDirection = DC_MOTOR_ROTATE_MINUS;
						DCMotor_Update(TPS_CALIBRATION_SPEED, tps_->calibrationDirection);
				}
				break;

			case DC_MOTOR_ROTATE_MINUS:
				if ((!isPlausible) && (tps_->calibOkCnt > TPS_MIN_CALIBRATION_PLAUSIBILITY_SAMPLES)) {
					/* DOWN calibration finished successfully */
					tps_->calibrationDirection = DC_MOTOR_DISABLED;
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
		if ((tps_->limits->calibMin < TPS_FEASIBLE_MIN) &&
			(tps_->limits->calibMax > TPS_FEASIBLE_MAX)) {
			/* Calibration OK */
			DCMotor_Enable();
			tps_->limits->min = (float)tps_->limits->calibMin;
			tps_->limits->max = (float)tps_->limits->calibMax;
			LED_SetStatus(LED_SOLID);
			nextState = DBW_RUN;
		} else {
			nextState = DBW_DISABLED_UNRECOVERABLE;
			tps_->error = ERROR_DBW_TPS_CALIBRATION;
		}
	}

	return nextState;
}

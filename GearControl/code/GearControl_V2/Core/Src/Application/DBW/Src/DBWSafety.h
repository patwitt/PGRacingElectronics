/*
 * DBW_Safety.h
 *
 *  Created on: 7 lip 2024
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_SRC_DBWSAFETY_H_
#define SRC_APPLICATION_DBW_SRC_DBWSAFETY_H_

#include "DBWTypes.h"

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

bool_t DBWSafety_CheckPowerOff(DbwHandle *const dbw_);
void DBWSafety_Plausibility(TpsSensorType *const tps, AppsSensorType *const apps);
ErrorEnum DBWSafety_Init(DbwHandle *const dbw_);

#endif /* SRC_APPLICATION_DBW_SRC_DBWSAFETY_H_ */

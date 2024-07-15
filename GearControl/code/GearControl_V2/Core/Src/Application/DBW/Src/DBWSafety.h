/*
 * DBW_Safety.h
 *
 *  Created on: 7 lip 2024
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_SRC_DBWSAFETY_H_
#define SRC_APPLICATION_DBW_SRC_DBWSAFETY_H_

#include "DBWTypes.h"

bool_t DBWSafety_CheckPowerOff(DbwHandle *const dbw_);
void DBWSafety_Plausibility(DBWSafety_PlausibilityType *const plausibility,
							SensorLimitsType *const limits,
							const uint16 sens1, const uint16 sens2,
							ErrorEnum *const error);

#endif /* SRC_APPLICATION_DBW_SRC_DBWSAFETY_H_ */

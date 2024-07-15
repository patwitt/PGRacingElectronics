/*
 * DBWCalibration.h
 *
 *  Created on: 7 lip 2024
 *      Author: lorem
 */

#ifndef SRC_APPLICATION_DBW_SRC_DBWCALIBRATION_H_
#define SRC_APPLICATION_DBW_SRC_DBWCALIBRATION_H_

#include "DBWTypes.h"

DBW_States DBWCalibration_APPS(AppsSensorType *const apps_);
DBW_States DBWCalibration_TPS(TpsSensorType *const tps_);

#endif /* SRC_APPLICATION_DBW_SRC_DBWCALIBRATION_H_ */

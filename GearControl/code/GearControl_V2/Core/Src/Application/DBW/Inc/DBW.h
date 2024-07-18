/*
 * GearControl.h
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_INC_DBW_H_
#define SRC_APPLICATION_DBW_INC_DBW_H_

#include "Types.h"
#include "DefineConfig.h"
#include "DBWTypes.h"

ErrorEnum DBW_Init(void);
void DBW_Process(void);
void DBW_RequestAppsCalibration(void);

#if CONFIG_ENABLE_THROTTLE_BLIP

float DBW_GetTargetPositionAbsError(void);
DbwRevMatchStatus DBW_RevMatchSetControl(float *const target);
void DBW_RevMatchRestoreNormalOperation(void);
#endif // CONFIG_ENABLE_THROTTLE_BLIP

#endif /* SRC_APPLICATION_DBW_INC_DBW_H_ */

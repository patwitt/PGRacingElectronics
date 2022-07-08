/*
 * GearControl.h
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_INC_DBW_H_
#define SRC_APPLICATION_DBW_INC_DBW_H_

#include "Types.h"

#define TPS_BIAS_OFFSET (14.0f)
#define TPS_IDLE (60.0f + TPS_BIAS_OFFSET) // 6%

ErrorEnum DBW_Init(void);
void DBW_Process(void);
void DBW_Disable(boolean isError);
void DBW_RequestAppsCalibration(void);
#endif /* SRC_APPLICATION_DBW_INC_DBW_H_ */

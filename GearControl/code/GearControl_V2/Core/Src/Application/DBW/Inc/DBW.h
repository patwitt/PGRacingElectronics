/*
 * GearControl.h
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_INC_DBW_H_
#define SRC_APPLICATION_DBW_INC_DBW_H_

#include "Types.h"

ErrorEnum DBW_Init(void);
void DBW_Process(void);
void DBW_Disable(boolean isError);
void DBW_RequestAppsCalibration(void);
#endif /* SRC_APPLICATION_DBW_INC_DBW_H_ */

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

#define TPS_BIAS_OFFSET (14.0f)
#define TPS_IDLE (50.0f + TPS_BIAS_OFFSET) // 6%

ErrorEnum DBW_Init(void);
void DBW_Process(void);
void DBW_Disable(boolean isError);
void DBW_RequestAppsCalibration(void);

#if CONFIG_ENABLE_REV_MATCH
typedef enum {
	REV_MATCH_DBW_FAILURE,
	REV_MATCH_TARGET_INVALID,
	REV_MATCH_DBW_OK
} DbwRevMatchStatus;

float DBW_GetTargetPositionAbsError(void);
DbwRevMatchStatus DBW_RevMatchSetControl(float *const target);
void DBW_RevMatchRestoreNormalOperation(void);
#endif // CONFIG_ENABLE_REV_MATCH

#endif /* SRC_APPLICATION_DBW_INC_DBW_H_ */

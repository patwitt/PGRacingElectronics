/*
 * ShiftRevMatch.h
 *
 *  Created on: 08.08.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_SHIFT_REV_MATCH_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_SHIFT_REV_MATCH_H_

#include "Types.h"
#include "DefineConfig.h"

#define SHIFT_REVMATCH_WATCHDOG_TIMEOUT_MS (100U) //!< Revmatch watchdog timeout in miliseconds
#define SHIFT_REVMATCH_ACCEPTABLE_POSITION_ERROR (15.0f) //!< 1.5%

typedef enum {
	REVMATCH_THROTTLE_BLIP,
	REVMATCH_POLL_TARGET,
	REVMATCH_FINISHED,
	REVMATCH_INACTIVE
} RevMatchStates;

ErrorEnum ShiftRevMatch_Init(void);
void ShiftRevMatch_Process(void);
void ShiftRevMatch_Trigger(const GearStates revMatchGear);
void ShiftRevMatch_Finish(void);
bool_t ShiftRevMatch_IsFinished(void);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_SHIFT_REV_MATCH_H_ */

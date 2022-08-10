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

ErrorEnum ShiftRevMatch_Init(void);
void ShiftRevMatch_Process(void);
void ShiftRevMatch_Trigger(const GearStates revMatchGear);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_SHIFT_REV_MATCH_H_ */

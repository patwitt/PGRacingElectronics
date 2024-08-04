/*
 * GearRevMatch.h
 *
 *  Created on: 4 sie 2024
 *
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_GEARREVMATCH_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_GEARREVMATCH_H_

#include "Types.h"
#include "SwTimer.h"

ErrorEnum GearRevMatch_Init(void);
void GearRevMatch_Trigger(const GearStates gear);
bool_t GearRevMatch_IsActive(void);
float GearRevMatch_GetTpsTarget(void);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARREVMATCH_H_ */

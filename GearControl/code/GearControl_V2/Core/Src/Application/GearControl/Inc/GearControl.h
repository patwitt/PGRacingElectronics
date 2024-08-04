/*
 * GearControl.h
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_INC_GEARCONTROL_H_
#define SRC_APPLICATION_GEARCONTROL_INC_GEARCONTROL_H_

#include "Types.h"
#include "stm32f4xx_hal.h"

ErrorEnum GearControl_Init(TIM_HandleTypeDef *const htim);
void GearControl_Process(void);
GearStates GearControl_GetGear(void);
bool_t GearControl_IsBypass(void);


#endif /* SRC_APPLICATION_GEARCONTROL_INC_GEARCONTROL_H_ */

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

#define GEAR_SHIFT_TIMEOUT_MS (300U)

typedef enum {
	/* ! This order must be unchanged ! begin */
	GEAR_1                = 0U,
	GEAR_N                = 1U,
	GEAR_2                = 2U,
	GEAR_3                = 3U,
	GEAR_4                = 4U,
	GEAR_5                = 5U,
	GEAR_6                = 6U,
	GEAR_UNKNOWN          = 7U,
	/* ! This order must be unchanged ! end */
	GEAR_INIT             = 8U,
	GEAR_DISABLED         = 9U,
	GEAR_SENS_FAILURE     = 10U,
	GEAR_SERVO_FAILURE    = 11U,
	GEAR_IMPLAUSIBLE      = 12U,
	GEAR_BYPASS           = 13U,
	GEAR_COUNT            = 14U
} GearStates;

ErrorEnum GearControl_Init(TIM_HandleTypeDef *const htim);
void GearControl_Process(void);
GearStates GearControl_GetGear(void);
bool_t GearControl_IsBypass(void);

#endif /* SRC_APPLICATION_GEARCONTROL_INC_GEARCONTROL_H_ */

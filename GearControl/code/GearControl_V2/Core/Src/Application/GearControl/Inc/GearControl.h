/*
 * GearControl.h
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_INC_GEARCONTROL_H_
#define SRC_APPLICATION_GEARCONTROL_INC_GEARCONTROL_H_

/*********** Macros ***********/

#define GEAR_ENABLE (1U)
#define GEAR_DISABLE (0U)

#define GEARSENS_THRESHOLD (50U)
#define GEARSENS_MIN (200U)
#define INIT_ATTEMPTS (10U)
#define SET_PWM (2U)
#define SET_DEG (4U)


#define DELAY_720MS (180U)
#define ARM_TIME_200MS (200UL)
#define SHIFT_TIMEOUT (100U)

#define GEAR_CUT_DELAY (4U)

#include "Types.h"

/*********** Data structures ***********/

typedef enum
{
	SHIFT_INIT = 0U,
	SHIFT_IDLE = 1U,
	SHIFT_PRELOAD = 2U,
	SHIFT_EXEC = 3U,
	SHIFT_DONE = 4U
}ShiftStates;

typedef enum
{
	INIT_SERVO = 0U,
	INIT_CAN_BUS = 1U,
	INIT_ENCODER = 2U,
	INIT_ADC = 3U,
	INIT_DONE = 4U
}InitStates;

typedef enum
{
	GEAR_SENS_IDLE,
	GEAR_SENS_UNKNOWN,
	GEAR_SENS_OK,
	GEAR_SENS_NOT_CHANGED
}GearSensorStates;

enum
{
	COL_BLACK = 0U,
	COL_GREEN = 1U,
	COL_RED = 2U,
	COL_ORANGE = 3U,
	COL_GREY = 4U
};

#define VALIDATION_POLL (2U)
#define VALIDATION_DONE (1U)
#define VALIDATION_END (0U)

#define ON (1U)
#define OFF (0U)

ErrorEnum GearControl_Init(TIM_HandleTypeDef *htim);
void GearControl_Process(void);

#endif /* SRC_APPLICATION_GEARCONTROL_INC_GEARCONTROL_H_ */

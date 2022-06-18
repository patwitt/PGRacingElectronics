/*
 * GearControl.h
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_GEARCONTROL_H_
#define INC_GEARCONTROL_H_

/*********** Macros ***********/

#define GEAR_ENABLE (1U)
#define GEAR_DISABLE (0U)

#define ALLOW_SHIFT_UP (1U)
#define ALLOW_SHIFT_DOWN (2U)
#define ALLOW_SHIFT_UP_DOWN (3U)

/* Shift requests */
#define REQUEST_EMPTY (0U)
#define REQUEST_SHIFT_UP (1U)
#define REQUEST_SHIFT_DOWN (2U)
#define REQUEST_SHIFT_N (3U)


#define GEARSENS_THRESHOLD (50U)
#define GEARSENS_MIN (200U)
#define INIT_ATTEMPTS (10U)
#define SET_PWM (2U)
#define SET_DEG (4U)

#define POS_PWM_DEFAULT (1500U) + 25
#define POS_PWM_MAX (1950U) + 25
#define POS_PWM_MIN (1050U) + 25

#define POS_DEG_MIN 0//(50U)
#define POS_DEG_DEFAULT (90U)
#define POS_DEG_MAX 180//(130U)

#define DELAY_720MS (180U)
#define ARM_TIME_200MS (200UL)
#define SHIFT_TIMEOUT (100U)

#define GEAR_CUT_DELAY (4U)

#define SHIFT_REQUEST_ERROR    (0x00000001UL)
#define POSITION_REQUEST_ERROR (0x00000002UL)
#define ENCODER_ERROR          (0x00000004UL)
#define TIMEOUT_ERROR          (0x00000008UL)
#define GEAR_NOT_SHIFTED_ERROR (0x00000010UL)
#define INIT_ENCODER_ERROR     (0x00000020UL)
#define INIT_GEARSENS_ERROR    (0x00000040UL)
#define OVERCURRENT_ERROR      (0x00000080UL)
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
	GEAR_INIT = 0,
	GEAR_1 = 1,
	GEAR_N = 2,
	GEAR_2 = 3,
	GEAR_3 = 4,
	GEAR_4 = 5,
	GEAR_5 = 6,
	GEAR_6 = 7,
	GEAR_IN_PROG = 8,
	GEAR_INVALID = 9
}GearStates;

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
	GEAR_IDLE,
	GEAR_UNKNOWN,
	GEAR_OK,
	GEAR_NOT_CHANGED
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

void GearControl_FuelCutTrigger(uint8);
boolean GearSensor_IsLocked(void);
void GearControl_SetControl(boolean);
boolean GearControl_IsActive(void);
ShiftStates ShiftControl_GetState(void);
void ShiftControl_SetState(ShiftStates state);
void GearControl_SetState(GearStates state);
void ShiftControl_SetRequest(uint8 request);
boolean GearControl_GetWdgStatus(void);
GearStates GearControl_GetNextState(void);
GearStates GearControl_GetState(void);
void GearControl_Init(__IO uint32*);
void GearControl_Process(void);
void GearControl_WdgProcess(void);
void GearControl_SetServoPos(uint8, uint8, uint32);
void GearControl_SetError(uint32, boolean);
void ShiftControl_SetValidation(boolean);
uint32 GearControl_GetErrors(void);
void GearSensor_SetState(GearSensorStates);
uint8 ShiftControl_GetValidation(void);
#endif /* INC_GEARCONTROL_H_ */

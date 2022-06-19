/*
 * DCMotorController.h
 *
 *  Created on: 18.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_MIDDLEWARE_DRIVERS_INC_DCMOTOR_H_
#define SRC_MIDDLEWARE_DRIVERS_INC_DCMOTOR_H_

#include "Types.h"

typedef enum {
	DC_MOTOR_ROTATE_MINUS    = 0U,
	DC_MOTOR_ROTATE_PLUS     = 1U,
	DC_MOTOR_ROTATE_DISABLED = 2U
} DCMotorDirectionEnum;

typedef enum {
	DC_MOTOR_UNINITIALIZED,
	DC_MOTOR_ENABLED,
	DC_MOTOR_DISABLED,
	DC_MOTOR_FAILURE
} DCMotorStateEnum;

ErrorEnum DCMotor_Init(void);
void DCMotor_Disable(void);
void DCMotor_Enable(void);
void DCMotor_Update(float target, DCMotorDirectionEnum direction);
__IO DCMotorStateEnum DCMotor_GetState(void);

#endif /* SRC_MIDDLEWARE_DRIVERS_INC_DCMOTOR_H_ */

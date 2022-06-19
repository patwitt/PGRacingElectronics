/*
 * Servo.h
 *
 *  Created on: 19.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_MIDDLEWARE_DRIVERS_INC_SERVO_H_
#define SRC_MIDDLEWARE_DRIVERS_INC_SERVO_H_

#include "Types.h"

typedef enum {
	SERVO_GEAR_SHIFT = 0U,
	SERVO_CLUTCH     = 1U,
	SERVO_COUNT      = 2U
} ServoTypeEnum;

typedef enum {
	SERVO_UNINITIALIZED,
	SERVO_ENABLED,
	SERVO_DISABLED,
	SERVO_FAILURE
} ServoStateEnum;

ErrorEnum Servo_Init(const ServoTypeEnum servo, __IO uint32_t *const pwm);
void Servo_Enable(const ServoTypeEnum servo);
void Servo_Disable(const ServoTypeEnum servo);
void Servo_SetPos(const ServoTypeEnum servo, const uint32_t deg);

#endif /* SRC_MIDDLEWARE_DRIVERS_INC_SERVO_H_ */

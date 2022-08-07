/*
 * Servo.h
 *
 *  Created on: 19.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_MIDDLEWARE_DRIVERS_INC_SERVO_H_
#define SRC_MIDDLEWARE_DRIVERS_INC_SERVO_H_

#include "Types.h"
#include "stm32f4xx_hal.h"

/* ---------------------------- */
typedef struct {
	const uint32_t degMin;
	const uint32_t degDefault;
	const uint32_t degMax;
} ServoPositionLimits;

typedef struct {
	const uint32_t pwmChannel;
	ServoPositionLimits limits;
} ServoConfig;

typedef struct {
	TIM_HandleTypeDef *htim;
	__IO uint32_t *PWM;
} ServoPwmParams;

typedef enum {
	SERVO_GEAR_SHIFT = 0U,
	SERVO_CLUTCH     = 1U,
	SERVO_COUNT      = 2U
} ServoEntityEnum;

typedef enum {
	SERVO_UNINITIALIZED,
	SERVO_ENABLED,
	SERVO_DISABLED,
	SERVO_FAILURE
} ServoStateEnum;

ErrorEnum Servo_Init(const ServoEntityEnum servoType, const ServoConfig *const config, ServoPwmParams pwmParams);
ErrorEnum Servo_Enable(const ServoEntityEnum servoType);
ErrorEnum Servo_EnableAndGoToDefaultPos(const ServoEntityEnum servoType);
ErrorEnum Servo_Disable(const ServoEntityEnum servoType);
ErrorEnum Servo_SetPos(const ServoEntityEnum servoType, const uint32_t deg);
ErrorEnum Servo_SetDefaultPos(const ServoEntityEnum servoType);

#endif /* SRC_MIDDLEWARE_DRIVERS_INC_SERVO_H_ */

/*
 * DCMotorController.c
 *
 *  Created on: 12.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "Servo.h"
#include <math.h>
#include "DefineConfig.h"
#include "Types.h"
#include "main.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
extern TIM_HandleTypeDef htim3;

typedef struct {
	const uint32_t degMin;
	const uint32_t degDefault;
	const uint32_t degMax;
} PositionLimits;

typedef struct {
	__IO uint32* PWM;
	__IO ServoStateEnum state;
	const uint32_t channel;
	PositionLimits *const limits;
} ServoController;

static PositionLimits clutchPosLim = {.degMin = 18U, .degDefault = 18U, .degMax = 162U};
static PositionLimits gearPosLim   = {.degMin = 50U, .degDefault = 90U, .degMax = 130U};

static ServoController servos[SERVO_COUNT] = {{.PWM = NULL, .state = SERVO_UNINITIALIZED, .channel = TIM_CHANNEL_3, .limits = &gearPosLim},    // Servo gear shift
											  {.PWM = NULL, .state = SERVO_UNINITIALIZED, .channel = TIM_CHANNEL_1, .limits = &clutchPosLim}}; // Servo clutch

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum Servo_Init(const ServoTypeEnum servo, __IO uint32_t *const pwm)
{
	ErrorEnum err = ERROR_OK;

	if (servo < SERVO_COUNT) {
		if ((htim3.Instance != NULL)  && (pwm != NULL)) {
			*pwm = 0U;
			servos[servo].PWM = pwm;
			servos[servo].state = SERVO_DISABLED;
		} else {
			err = ERROR_NULL;
		}

		if (err != ERROR_OK) {
			servos[servo].state = SERVO_FAILURE;
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

void Servo_Disable(const ServoTypeEnum servo)
{
	if (servo < SERVO_COUNT) {
		if (servos[servo].PWM != NULL) {
			*servos[servo].PWM = 0U;
			servos[servo].state = SERVO_DISABLED;

			/* Stop PWM timer */
			if (HAL_TIM_PWM_Stop(&htim3, servos[servo].channel) != HAL_OK) {
				servos[servo].state = SERVO_FAILURE;
			}
		}
	}
}

void Servo_Enable(const ServoTypeEnum servo)
{
	if (servo < SERVO_COUNT) {
		if (servos[servo].PWM != NULL) {
			*servos[servo].PWM = 0U;
			servos[servo].state = SERVO_ENABLED;

			/* Start PWM timer */
			if (HAL_TIM_PWM_Start(&htim3, servos[servo].channel) != HAL_OK) {
				servos[servo].state = SERVO_FAILURE;
			}
		}
	}
}

/* 500 - 0 deg */
/* 1500 - 90 deg */
/* 2500 - 180 deg */
void Servo_SetPos(const ServoTypeEnum servo, const uint32_t deg)
{
	if (servo < SERVO_COUNT) {
		switch (servos[servo].state) {
		case SERVO_ENABLED:
			if ((deg >= servos[servo].limits->degMin) && (deg <= servos[servo].limits->degMax)) {
				if (deg < 90U) {
					/* 0-90 deg use floor */
					*servos[servo].PWM = (uint32)(floor((11.111111f * deg))) + (uint32)500U;
				} else {
					/* 90-180 deg use ceil */
					*servos[servo].PWM = (uint32)(ceil((11.111111f * deg))) + (uint32)500U;
				}
			}
			break;

		case SERVO_DISABLED:
			/* Do nothing */
			break;

		case SERVO_UNINITIALIZED:
		case SERVO_FAILURE:
		default:
			Servo_Disable(servo);
			break;
		}
	}
}

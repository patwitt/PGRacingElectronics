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
#include "Utils.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

typedef struct {
	__IO ServoStateEnum state;
	const ServoConfig *config;
	ServoPwmParams pwm;
} ServoController;

static ServoController servos[SERVO_COUNT] = {{.state = SERVO_UNINITIALIZED},  // Servo gear shift
											  {.state = SERVO_UNINITIALIZED}   // Servo clutch
};

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum Servo_Init(const ServoTypeEnum servoType, const ServoConfig *const config, ServoPwmParams pwmParams)
{
	ErrorEnum err = ERROR_OK;

	if (servoType < SERVO_COUNT) {
		ServoController *const servo = &servos[servoType];

		NULL_ERR_CHECK3(err, config, pwmParams.htim, pwmParams.PWM);

		if (err == ERROR_OK) {
			servo->config = config;
			servo->pwm = pwmParams;

			/* Disable servo at Init */
			Servo_Disable(servoType);
		} else {
			servo->state = SERVO_FAILURE;
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

ErrorEnum Servo_SetDefaultPos(const ServoTypeEnum servoType)
{
	ErrorEnum err = ERROR_OK;

	if (servoType < SERVO_COUNT) {
		ServoController *const servo = &servos[servoType];

		NULL_ERR_CHECK1(err, servo->config);

		if (err == ERROR_OK) {
			err = Servo_SetPos(servoType, servo->config->limits.degDefault);
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

ErrorEnum Servo_Disable(const ServoTypeEnum servoType)
{
	ErrorEnum err = ERROR_OK;

	if (servoType < SERVO_COUNT) {
		ServoController *const servo = &servos[servoType];

		if (servo->state != SERVO_DISABLED) {
			NULL_ERR_CHECK3(err, servo->config, servo->pwm.htim, servo->pwm.PWM);

			if (err == ERROR_OK) {
				*servo->pwm.PWM = 0U;

				/* Stop PWM timer */
				if (HAL_TIM_PWM_Stop(servo->pwm.htim, servo->config->pwmChannel) == HAL_OK) {
					/* Disabling OK */
					servo->state = SERVO_DISABLED;
				} else {
					servo->state = SERVO_FAILURE;
				}
			}
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

ErrorEnum Servo_EnableAndGoToDefaultPos(const ServoTypeEnum servoType)
{
	ErrorEnum err = Servo_Enable(servoType);

	if (err == ERROR_OK) {
		err = Servo_SetDefaultPos(servoType);
	}

	return err;
}

ErrorEnum Servo_Enable(const ServoTypeEnum servoType)
{
	ErrorEnum err = ERROR_OK;

	if (servoType < SERVO_COUNT) {
		ServoController *const servo = &servos[servoType];

		if (servo->state != SERVO_ENABLED) {
			NULL_ERR_CHECK3(err, servo->config, servo->pwm.htim, servo->pwm.PWM);

			if (err == ERROR_OK) {
				*servo->pwm.PWM = 0U;

				/* Start PWM timer */
				if (HAL_TIM_PWM_Start(servo->pwm.htim, servo->config->pwmChannel) == HAL_OK) {
					/* Enabling OK */
					servo->state = SERVO_ENABLED;
				} else {
					servo->state = SERVO_FAILURE;
				}
			}
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

/* 500 - 0 deg */
/* 1500 - 90 deg */
/* 2500 - 180 deg */
ErrorEnum Servo_SetPos(const ServoTypeEnum servoType, const uint32_t deg)
{
	ErrorEnum err = ERROR_OK;

	if (servoType < SERVO_COUNT) {
		ServoController *const servo = &servos[servoType];

		switch (servo->state) {
		case SERVO_ENABLED:
			if ((deg >= servo->config->limits.degMin) &&
				(deg <= servo->config->limits.degMax)) {
				if (deg < 90U) {
					/* 0-90 deg use floor */
					*servo->pwm.PWM = (uint32)(floor((11.111111f * deg))) + (uint32)500U;
				} else {
					/* 90-180 deg use ceil */
					*servo->pwm.PWM = (uint32)(ceil((11.111111f * deg))) + (uint32)500U;
				}
			}
			break;

		case SERVO_DISABLED:
			/* Do nothing */
			break;

		case SERVO_UNINITIALIZED:
		case SERVO_FAILURE:
		default:
			err = Servo_Disable(servoType);
			break;
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

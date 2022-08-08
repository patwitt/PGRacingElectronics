/*
 * Servo.c
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
#include "SwTimer.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

typedef struct {
	__IO ServoStateEnum state;
	const ServoConfig *config;
	ServoPwmParams pwm;
} ServoController;

static ServoController servos[SERVO_COUNT] = {
		[SERVO_GEAR_SHIFT] = {
			.state = SERVO_UNINITIALIZED
		},
		[SERVO_CLUTCH] = {
			.state = SERVO_UNINITIALIZED
		}
};

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
/**
 * @brief Initialization of the servo controller.
 * 
 * @param servoEntity The servo entity that will be initialized.
 * @param config      Pointer to a ServoConfig struct, which contains the following:
 * @param pwmParams   This is a struct that contains the timer and the channel that the servo is
 * connected to.
 * 
 * @return an error code.
 */
ErrorEnum Servo_Init(const ServoEntityEnum servoEntity, const ServoConfig *const config, ServoPwmParams pwmParams)
{
	ErrorEnum err = ERROR_OK;

	if (servoEntity < SERVO_COUNT) {
		ServoController *const servo = &servos[servoEntity];

		NULL_ERR_CHECK3(err, config, pwmParams.htim, pwmParams.PWM);

		if (err == ERROR_OK) {
			servo->config = config;
			servo->pwm = pwmParams;

			/* Disable servo at Init */
			Servo_Disable(servoEntity);
		} else {
			servo->state = SERVO_FAILURE;
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

/**
 * @brief Set the servo to its default position.
 * 
 * @param servoEntity The servo entity to set the position of.
 * 
 * @return an error code.
 */
ErrorEnum Servo_SetDefaultPos(const ServoEntityEnum servoEntity)
{
	ErrorEnum err = ERROR_OK;

	if (servoEntity < SERVO_COUNT) {
		ServoController *const servo = &servos[servoEntity];

		NULL_ERR_CHECK1(err, servo->config);

		if (err == ERROR_OK) {
			err = Servo_SetPos(servoEntity, servo->config->limits.degDefault);
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

/**
 * @brief Disable the PWM output of the servo.
 * 
 * @param servoEntity The servo entity to disable.
 * 
 * @return an error code.
 */
ErrorEnum Servo_Disable(const ServoEntityEnum servoEntity)
{
	ErrorEnum err = ERROR_OK;

	if (servoEntity < SERVO_COUNT) {
		ServoController *const servo = &servos[servoEntity];

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

/**
 * @brief Enable the servo and go to the default position.
 * 
 * @param servoEntity The servo entity to perform action.
 * 
 * @return an error code.
 */
ErrorEnum Servo_EnableAndGoToDefaultPos(const ServoEntityEnum servoEntity)
{
	ErrorEnum err = Servo_Enable(servoEntity);

	if (err == ERROR_OK) {
		err = Servo_SetDefaultPos(servoEntity);
	}

	return err;
}

/**
 * @brief Enable a servo by starting the PWM timer.
 * 
 * @param servoEntity The servo entity to enable.
 * 
 * @return an error code.
 */
ErrorEnum Servo_Enable(const ServoEntityEnum servoEntity)
{
	ErrorEnum err = ERROR_OK;

	if (servoEntity < SERVO_COUNT) {
		ServoController *const servo = &servos[servoEntity];

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
/**
 * @brief Set servo position.
 * 
 * If the servo is enabled, and the requested position is within the servo's limits, then set the PWM
 * value to the requested position.
 *
 * The function is pretty simple, but there are a few things to note:
 * 1. The function takes a servo type and a position in degrees as parameters.
 * 2. The function checks if the servo is enabled. If it is not, then the function returns an error.
 * 3. The function checks if the requested position is within the servo's limits. If it is not, then
 * the function returns an error.
 * 
 * @param servoEntity The servo entity to set position.
 * @param deg         Servo degrees in range 0-180.
 * 
 * @return an error code.
 */
ErrorEnum Servo_SetPos(const ServoEntityEnum servoEntity, const uint32_t deg)
{
	ErrorEnum err = ERROR_NOK;

	if (servoEntity < SERVO_COUNT) {
		ServoController *const servo = &servos[servoEntity];

		switch (servo->state) {
		case SERVO_ENABLED:
			if ((deg >= servo->config->limits.degMin) &&
				(deg <= servo->config->limits.degMax)) {
				/* Valid set pos request */
				err = ERROR_OK;
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
			(void)Servo_Disable(servoEntity);
			break;
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}
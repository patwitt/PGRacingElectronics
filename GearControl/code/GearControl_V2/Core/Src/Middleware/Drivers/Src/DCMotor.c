/*
 * DCMotorController.c
 *
 *  Created on: 12.06.2022
 *      Author: Patryk Wittbrodt
 */

#include <DCMotor.h>
#include "Utils.h"
#include "Types.h"
#include "main.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
#if 0
/* Max. frequency available - 25kHz */
#define PWM_DUTY_CYCLE_MAX (3359.0f)
#define PWM_MULTIPLIER (3359.0f / 1000.0f)
#endif

#define PWM_DUTY_CYCLE_MAX (4640.0f)
#define PWM_MULTIPLIER (4640.0f / 1000.0f)
#define PWM_DUTY_CYCLE_MIN (0U)

extern TIM_HandleTypeDef htim1;

typedef struct {
	__IO uint32* PWM_H_plus;
	__IO uint32* PWM_H_minus;
	__IO uint32 targetDutyCycle;
	__IO DCMotorStateEnum state;
} DCMotorController;

static DCMotorController dcController = {.PWM_H_plus = NULL, .PWM_H_minus = NULL, .targetDutyCycle = 0U, .state = DC_MOTOR_UNINITIALIZED};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static void DCMotor_ResetPwm(void);
static __IO uint32 DCMotor_ConvertTargetToPwm(float target);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */
static void DCMotor_ResetPwm(void) {
	*dcController.PWM_H_plus = 0U;
	*dcController.PWM_H_minus = 0U;
}

static __IO uint32 DCMotor_ConvertTargetToPwm(float target) {
	float pwmF = target * PWM_MULTIPLIER;
	pwmF = CLAMP_MAX(pwmF, PWM_DUTY_CYCLE_MAX);
	pwmF = CLAMP_MIN(pwmF, PWM_DUTY_CYCLE_MIN);
	return (uint32)pwmF;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

/**
 * @brief Initialization of the DC Motor module.
 * 
 * It initializes the PWM timers for the DC motor and sets the PWM_H_plus and PWM_H_minus pointers to
 * the correct memory locations.
 * 
 * @return an error code.
 */
ErrorEnum DCMotor_Init(void) {
	ErrorEnum err = ERROR_OK;

	/* Start PWM timers for ETC */
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK) {
	  err = ERROR_HAL;
	}

	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4) != HAL_OK) {
	  err = ERROR_HAL;
	}

	if (err == ERROR_OK) {
		if (htim1.Instance != NULL) {
			  dcController.PWM_H_plus = &htim1.Instance->CCR4;
			  dcController.PWM_H_minus = &htim1.Instance->CCR1;
			  DCMotor_Disable();
			  if (dcController.state != DC_MOTOR_DISABLED) {
				  err = ERROR_DBW_DC_MOTOR_INIT;
			  }
		} else {
			err = ERROR_NULL;
		}
	}

	if (err != ERROR_OK) {
		dcController.state = DC_MOTOR_FAILURE;
	}

	return err;
}

/**
 * @brief DC Motor update.
 * 
 * The function takes a target speed and a direction as input, and then sets the duty cycle of the PWM
 * output to the target speed.
 * 
 * @param target    The target speed of the motor from PID controller.
 * @param direction DC Motor rotational direction.
 */
void DCMotor_Update(float target, DCMotorDirectionEnum direction)
{
	switch (dcController.state) {
		case DC_MOTOR_ENABLED:
			dcController.targetDutyCycle = DCMotor_ConvertTargetToPwm(target);

			switch (direction) {
				case DC_MOTOR_ROTATE_PLUS:
					*dcController.PWM_H_minus = 0U;
					*dcController.PWM_H_plus = dcController.targetDutyCycle;
					break;

				case DC_MOTOR_ROTATE_MINUS:
					*dcController.PWM_H_plus = 0U;
					*dcController.PWM_H_minus = dcController.targetDutyCycle;
					break;

				case DC_MOTOR_ROTATE_DISABLED:
				default:
					*dcController.PWM_H_minus = 0U;
					*dcController.PWM_H_plus = 0U;
					break;
			}
			break;

		default:
			/* In any other state, disable motor */
			DCMotor_Disable();
			break;
	}
}

/**
 * @brief Disable the DC Motor.
 * 
 * The function disables the motor by resetting the PWM,
 * setting the OE, INH1, INH2 pin to low.
 */
void DCMotor_Disable(void)
{
	DCMotor_ResetPwm();

	HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(INH1_GPIO_Port, INH1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(INH2_GPIO_Port, INH2_Pin, GPIO_PIN_RESET);

	if ((HAL_GPIO_ReadPin(OE_GPIO_Port, OE_Pin) == GPIO_PIN_RESET) &&
	   (HAL_GPIO_ReadPin(INH1_GPIO_Port, INH1_Pin) == GPIO_PIN_RESET) &&
	   (HAL_GPIO_ReadPin(INH2_GPIO_Port, INH2_Pin) == GPIO_PIN_RESET))
	{
		dcController.state = DC_MOTOR_DISABLED;
	} else {
		dcController.state = DC_MOTOR_FAILURE;
	}
}

/**
 * @brief Enable the DC Motor.
 * 
 * The function enables the motor by resetting the PWM,
 * setting the OE, INH1, INH2 pin to high.
 */
void DCMotor_Enable(void)
{
	DCMotor_ResetPwm();

	/* Enable Throttle Body */
	if (dcController.state == DC_MOTOR_DISABLED) {
		HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(INH1_GPIO_Port, INH1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(INH2_GPIO_Port, INH2_Pin, GPIO_PIN_SET);

		if ((HAL_GPIO_ReadPin(OE_GPIO_Port, OE_Pin) == GPIO_PIN_SET) &&
		   (HAL_GPIO_ReadPin(INH1_GPIO_Port, INH1_Pin) == GPIO_PIN_SET) &&
		   (HAL_GPIO_ReadPin(INH2_GPIO_Port, INH2_Pin) == GPIO_PIN_SET))
		{
			dcController.state = DC_MOTOR_ENABLED;
		} else {
			dcController.state = DC_MOTOR_FAILURE;
		}
	}
}

__IO DCMotorStateEnum DCMotor_GetState(void) {
	return dcController.state;
}

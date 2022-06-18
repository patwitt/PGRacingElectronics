/*
 * DCMotorController.c
 *
 *  Created on: 12.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "DefineConfig.h"
#include "DCMotorController.h"
#include "Types.h"
#include "main.h"


#define PWM_DUTY_CYCLE_MAX (1000U)
#define PWM_MULTIPLIER (3359.0f / 1000.0f)
#define PWM_DUTY_CYCLE_MIN (0U)

extern TIM_HandleTypeDef htim1;

typedef struct {
	__IO uint32* PWM_H_plus;
	__IO uint32* PWM_H_minus;
} DCMotorController;

DCMotorController dcController = {.PWM_H_plus = NULL, .PWM_H_minus = NULL};

static void DCMotorController_ResetPwm(void);
static uint32 DCMotorController_ConvertTargetToPwm(float target);

static void DCMotorController_ResetPwm(void) {
	*dcController.PWM_H_plus = 0U;
	*dcController.PWM_H_minus = 0U;
}

static uint32 DCMotorController_ConvertTargetToPwm(float target) {
	float pwmF = target * PWM_MULTIPLIER;
	uint32 pwm_ = (uint32)pwmF;
	pwm_ = CLAMP_MAX(pwm_, PWM_DUTY_CYCLE_MAX);
	pwm_ = CLAMP_MIN(pwm_, PWM_DUTY_CYCLE_MIN);
	return pwm_;
}

ErrorEnum DCMotorController_Init(void) {
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
			  dcController.PWM_H_plus = &htim1.Instance->CCR1;
			  dcController.PWM_H_minus = &htim1.Instance->CCR4;
			  DCMotorController_Disable();
		} else {
			err = ERROR_NULL;
		}
	}

	return err;
}

void DCMotorController_Update(float target, DCMotorDirectionEnum direction)
{
	switch (direction) {
		case DC_MOTOR_ROTATE_PLUS:
			*dcController.PWM_H_minus = PWM_DUTY_CYCLE_MIN;
			*dcController.PWM_H_plus = DCMotorController_ConvertTargetToPwm(target);
			break;

		case DC_MOTOR_ROTATE_MINUS:
			*dcController.PWM_H_plus = PWM_DUTY_CYCLE_MIN;
			*dcController.PWM_H_minus = DCMotorController_ConvertTargetToPwm(target);
			break;

		case DC_MOTOR_DISABLED:
		default:
			*dcController.PWM_H_minus = PWM_DUTY_CYCLE_MIN;
			*dcController.PWM_H_plus = PWM_DUTY_CYCLE_MIN;
			break;
	}
}

void DCMotorController_Disable(void)
{
	HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(INH1_GPIO_Port, INH1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(INH2_GPIO_Port, INH2_Pin, GPIO_PIN_RESET);

	DCMotorController_ResetPwm();
}

void DCMotorController_Enable(void)
{
	/* Enable Throttle Body */
	HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(INH1_GPIO_Port, INH1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(INH2_GPIO_Port, INH2_Pin, GPIO_PIN_SET);

	DCMotorController_ResetPwm();
}

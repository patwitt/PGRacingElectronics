/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_STATUS_Pin GPIO_PIN_2
#define LED_STATUS_GPIO_Port GPIOA
#define TPS_1_Pin GPIO_PIN_5
#define TPS_1_GPIO_Port GPIOA
#define TPS_2_Pin GPIO_PIN_7
#define TPS_2_GPIO_Port GPIOA
#define APPS_1_Pin GPIO_PIN_5
#define APPS_1_GPIO_Port GPIOC
#define APPS_2_Pin GPIO_PIN_1
#define APPS_2_GPIO_Port GPIOB
#define GEAR_CUT_Pin GPIO_PIN_14
#define GEAR_CUT_GPIO_Port GPIOB
#define PWM_CLUTCH_Pin GPIO_PIN_6
#define PWM_CLUTCH_GPIO_Port GPIOC
#define PWM_GEAR_Pin GPIO_PIN_8
#define PWM_GEAR_GPIO_Port GPIOC
#define OE_Pin GPIO_PIN_9
#define OE_GPIO_Port GPIOC
#define PWM_H_IN2_Pin GPIO_PIN_8
#define PWM_H_IN2_GPIO_Port GPIOA
#define INH2_Pin GPIO_PIN_9
#define INH2_GPIO_Port GPIOA
#define INH1_Pin GPIO_PIN_10
#define INH1_GPIO_Port GPIOA
#define PWM_H_IN1_Pin GPIO_PIN_11
#define PWM_H_IN1_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define RUN_DEBUG
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

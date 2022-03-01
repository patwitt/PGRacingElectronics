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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ROT_UNI_Pin GPIO_PIN_0
#define ROT_UNI_GPIO_Port GPIOA
#define ROT_SCR_Pin GPIO_PIN_1
#define ROT_SCR_GPIO_Port GPIOA
#define LED_STATUS_Pin GPIO_PIN_2
#define LED_STATUS_GPIO_Port GPIOA
#define BTN_SCR_Pin GPIO_PIN_4
#define BTN_SCR_GPIO_Port GPIOC
#define BTN_PTT_Pin GPIO_PIN_5
#define BTN_PTT_GPIO_Port GPIOC
#define BTN_CLCH_Pin GPIO_PIN_0
#define BTN_CLCH_GPIO_Port GPIOB
#define BTN_UNI1_Pin GPIO_PIN_1
#define BTN_UNI1_GPIO_Port GPIOB
#define BTN_DRS_Pin GPIO_PIN_2
#define BTN_DRS_GPIO_Port GPIOB
#define BTN_UNI2_Pin GPIO_PIN_6
#define BTN_UNI2_GPIO_Port GPIOC
#define LED_DATA_Pin GPIO_PIN_8
#define LED_DATA_GPIO_Port GPIOC
#define I2C_RESET_Pin GPIO_PIN_5
#define I2C_RESET_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

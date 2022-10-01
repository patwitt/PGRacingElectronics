/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l4xx_hal.h"

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
#define PTT_Pin GPIO_PIN_0
#define PTT_GPIO_Port GPIOC
#define Damper_1_Pin GPIO_PIN_0
#define Damper_1_GPIO_Port GPIOA
#define Damper_2_Pin GPIO_PIN_1
#define Damper_2_GPIO_Port GPIOA
#define LED_Status_Pin GPIO_PIN_2
#define LED_Status_GPIO_Port GPIOA
#define BP_Front_Pin GPIO_PIN_3
#define BP_Front_GPIO_Port GPIOA
#define BP_Rear_Pin GPIO_PIN_4
#define BP_Rear_GPIO_Port GPIOA
#define Additional_1_Pin GPIO_PIN_7
#define Additional_1_GPIO_Port GPIOC
#define Additional_2_Pin GPIO_PIN_8
#define Additional_2_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

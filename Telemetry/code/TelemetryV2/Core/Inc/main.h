/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f7xx_hal.h"

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
#define ADC_DATA_RDY_Pin GPIO_PIN_3
#define ADC_DATA_RDY_GPIO_Port GPIOE
#define ADC_CS_Pin GPIO_PIN_4
#define ADC_CS_GPIO_Port GPIOE
#define ABS_SPARE2_Pin GPIO_PIN_1
#define ABS_SPARE2_GPIO_Port GPIOB
#define GPS_LNA_EN_Pin GPIO_PIN_5
#define GPS_LNA_EN_GPIO_Port GPIOG
#define GPS_INT_Pin GPIO_PIN_7
#define GPS_INT_GPIO_Port GPIOC
#define USB_SW_Pin GPIO_PIN_10
#define USB_SW_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOD
#define SD_DET_Pin GPIO_PIN_13
#define SD_DET_GPIO_Port GPIOG
#define ADC_CLK_Pin GPIO_PIN_9
#define ADC_CLK_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

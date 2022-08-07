/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "app_touchgfx.h"
#include "can.h"
#include "gpio.h"
#include "tim.h"
#include "WS2812_driver.h"
#include "ecumaster.h"
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for hardwareTASK */
osThreadId_t hardwareTASKHandle;
const osThreadAttr_t hardwareTASK_attributes = {
  .name = "hardwareTASK",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for touchGFXTask */
osThreadId_t touchGFXTaskHandle;
const osThreadAttr_t touchGFXTask_attributes = {
  .name = "touchGFXTask",
  .stack_size = 32768 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for infoLed */
osThreadId_t infoLedHandle;
const osThreadAttr_t infoLed_attributes = {
  .name = "infoLed",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for displayBackligh */
osThreadId_t displayBacklighHandle;
const osThreadAttr_t displayBackligh_attributes = {
  .name = "displayBackligh",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartHardwareTask(void *argument);
void StartTouchGFXTask(void *argument);
void StartInfoLed(void *argument);
void StartDisplayBacklight(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of hardwareTASK */
  hardwareTASKHandle = osThreadNew(StartHardwareTask, NULL, &hardwareTASK_attributes);

  /* creation of touchGFXTask */
  touchGFXTaskHandle = osThreadNew(StartTouchGFXTask, NULL, &touchGFXTask_attributes);

  /* creation of infoLed */
  infoLedHandle = osThreadNew(StartInfoLed, NULL, &infoLed_attributes);

  /* creation of displayBackligh */
  displayBacklighHandle = osThreadNew(StartDisplayBacklight, NULL, &displayBackligh_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartHardwareTask */
/**
 * @brief  Function implementing the hardwareTASK thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartHardwareTask */
void StartHardwareTask(void *argument)
{
  /* USER CODE BEGIN StartHardwareTask */

	/*MX_TIM3_Init();
	MX_DMA_Init();
	uint16_t rpm = 5000;
	leds_init();
	osDelay(1000);*/
	 //MX_CAN2_Init();
	osDelay(10);

	HAL_CAN_Start(&hcan2);
	//HAL_CAN_Start(&hcan1);

	osDelay(10);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	//HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	/* Infinite loop */
	for (;;) {
		/*light_leds(rpm,1);
		rpm+=100;
		if(rpm > 12000)
		{
			rpm = 4000;
		}*/
		//EcuData.rpm+=100;
		osDelay(100);
	}
  /* USER CODE END StartHardwareTask */
}

/* USER CODE BEGIN Header_StartTouchGFXTask */
/**
 * @brief Function implementing the touchGFXTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTouchGFXTask */
void StartTouchGFXTask(void *argument)
{
  /* USER CODE BEGIN StartTouchGFXTask */
	MX_TouchGFX_Process();
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END StartTouchGFXTask */
}

/* USER CODE BEGIN Header_StartInfoLed */
/**
* @brief Function implementing the infoLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartInfoLed */
void StartInfoLed(void *argument)
{
  /* USER CODE BEGIN StartInfoLed */
  /* Infinite loop */
  for(;;)
  {
    osDelay(250);
    HAL_GPIO_TogglePin(SIGNAL_LED_GPIO_Port, SIGNAL_LED_Pin);
   // HAL_IWDG_Refresh(&hiwdg);
  }
  /* USER CODE END StartInfoLed */
}

/* USER CODE BEGIN Header_StartDisplayBacklight */
/**
* @brief Function implementing the displayBackligh thread.
* @param argument: Not used
* @retval None
*/
uint16_t data2;
float data3;
float data4;
uint8_t data;
/* USER CODE END Header_StartDisplayBacklight */
void StartDisplayBacklight(void *argument)
{
  /* USER CODE BEGIN StartDisplayBacklight */
  /* Infinite loop */

	//MX_TIM4_Init();
	//MX_ADC1_Init();


	uint8_t setPoint = 100;
	// HAL_TIM_Base_Start(&htim4);

	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	  osDelay(250);
  for(;;)
  {
	  HAL_ADC_Start(&hadc1);

	  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, data);

	 // data+=50;
		//	  data%= 200;
	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

	  osDelay(100);
	  data2 = HAL_ADC_GetValue(&hadc1);
	  data3 = 3.3f * data2 /4095.0f;
	  data4 = data3 / 5.1f * 1000.0f;

	  data+= (int8_t)round((setPoint- data4)/ 10.0f);
	  if(data>215)
		  data = 215;

  }
  /* USER CODE END StartDisplayBacklight */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


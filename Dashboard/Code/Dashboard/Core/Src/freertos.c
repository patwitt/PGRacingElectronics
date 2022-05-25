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
#include "app_touchgfx.h"
#include "can.h"
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
const osThreadAttr_t hardwareTASK_attributes = { .name = "hardwareTASK",
		.stack_size = 512 * 4, .priority = (osPriority_t) osPriorityNormal, };
/* Definitions for touchGFXTask */
osThreadId_t touchGFXTaskHandle;
const osThreadAttr_t touchGFXTask_attributes = { .name = "touchGFXTask",
		.stack_size = 8192 * 4, .priority = (osPriority_t) osPriorityNormal, };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartHardwareTask(void *argument);
void StartTouchGFXTask(void *argument);

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
	hardwareTASKHandle = osThreadNew(StartHardwareTask, NULL,
			&hardwareTASK_attributes);

	/* creation of touchGFXTask */
	touchGFXTaskHandle = osThreadNew(StartTouchGFXTask, NULL,
			&touchGFXTask_attributes);

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
void StartHardwareTask(void *argument) {
	/* USER CODE BEGIN StartHardwareTask */

	/* Infinite loop */
	for (;;) {
		osDelay(1);
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
void StartTouchGFXTask(void *argument) {
	/* USER CODE BEGIN StartTouchGFXTask */
	MX_TouchGFX_Process();
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartTouchGFXTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


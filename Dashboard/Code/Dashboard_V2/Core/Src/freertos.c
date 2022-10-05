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
#include "structs/ecumaster.h"
#include "structs/display_data.h"
#include "structs/telemetry_data.h"
#include "tim.h"
#include "WS2812_driver.h"
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PWM_CHANGE 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
DisplaySetup_t displaySetup =
{ 70, 2, 110, 110, 110 };
EcumasterData_t ecumasterData;
TelemetryData_t telemetryData;

/* USER CODE END Variables */
/* Definitions for hardwareTask */
osThreadId_t hardwareTaskHandle;
const osThreadAttr_t hardwareTask_attributes = {
  .name = "hardwareTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for RPMLed */
osThreadId_t RPMLedHandle;
const osThreadAttr_t RPMLed_attributes = {
  .name = "RPMLed",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for touchGFXTask */
osThreadId_t touchGFXTaskHandle;
const osThreadAttr_t touchGFXTask_attributes = {
  .name = "touchGFXTask",
  .stack_size = 32768 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for statusLedTask */
osThreadId_t statusLedTaskHandle;
const osThreadAttr_t statusLedTask_attributes = {
  .name = "statusLedTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for DisplayDashboar */
osThreadId_t DisplayDashboarHandle;
const osThreadAttr_t DisplayDashboar_attributes = {
  .name = "DisplayDashboar",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartHardwareTask(void *argument);
void startRPMLed(void *argument);
void startTouchGFXTask(void *argument);
void startStatusLedTask(void *argument);
void StartDisplayDashoard(void *argument);

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
  /* creation of hardwareTask */
  hardwareTaskHandle = osThreadNew(StartHardwareTask, NULL, &hardwareTask_attributes);

  /* creation of RPMLed */
  RPMLedHandle = osThreadNew(startRPMLed, NULL, &RPMLed_attributes);

  /* creation of touchGFXTask */
  touchGFXTaskHandle = osThreadNew(startTouchGFXTask, NULL, &touchGFXTask_attributes);

  /* creation of statusLedTask */
  statusLedTaskHandle = osThreadNew(startStatusLedTask, NULL, &statusLedTask_attributes);

  /* creation of DisplayDashboar */
  DisplayDashboarHandle = osThreadNew(StartDisplayDashoard, NULL, &DisplayDashboar_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartHardwareTask */
/**
 * @brief  Function implementing the hardwareTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartHardwareTask */
void StartHardwareTask(void *argument)
{
  /* USER CODE BEGIN StartHardwareTask */
	CanCommunicationInit();
	/* Infinite loop */
	for (;;)
	{
		osDelay(1);
	}
  /* USER CODE END StartHardwareTask */
}

/* USER CODE BEGIN Header_startRPMLed */
/**
 * @brief Function implementing the RPMLed thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startRPMLed */
void startRPMLed(void *argument)
{
  /* USER CODE BEGIN startRPMLed */
	WS2812_Init();
	/* Infinite loop */
	for (;;)
	{
		uint8_t displayMode = telemetryData.gear == 0 ? 0 : displaySetup.ledBarMode;
		updateLeds(ecumasterData.rpm, displayMode);
		osDelay(50);
	}
  /* USER CODE END startRPMLed */
}

/* USER CODE BEGIN Header_startTouchGFXTask */
/**
 * @brief Function implementing the touchGFXTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startTouchGFXTask */
void startTouchGFXTask(void *argument)
{
  /* USER CODE BEGIN startTouchGFXTask */
	/* Infinite loop */
	for (;;)
	{
		osDelay(1);
	}
  /* USER CODE END startTouchGFXTask */
}

/* USER CODE BEGIN Header_startStatusLedTask */
/**
 * @brief Function implementing the statusLedTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startStatusLedTask */
void startStatusLedTask(void *argument)
{
  /* USER CODE BEGIN startStatusLedTask */
	/* Infinite loop */
	for (;;)
	{
		osDelay(1);
	}
  /* USER CODE END startStatusLedTask */
}

/* USER CODE BEGIN Header_StartDisplayDashoard */
/**
* @brief Function implementing the DisplayDashboar thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDisplayDashoard */
void StartDisplayDashoard(void *argument)
{
  /* USER CODE BEGIN StartDisplayDashoard */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDisplayDashoard */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


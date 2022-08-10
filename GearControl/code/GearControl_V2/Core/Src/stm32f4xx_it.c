/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DefineConfig.h"
#include "CAN.h"
#include "Adc.h"
#include "Scheduler.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

static AdcDataChannel* adc1Channels = NULL;
static AdcDataChannel* adc2Channels = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if CONFIG_RUN_DEBUG
uint16_t debugAdc1[ADC_1_CHANNELS_COUNT];
uint16_t debugAdc2[ADC_2_CHANNELS_COUNT];
#endif
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern CAN_HandleTypeDef hcan1;
/* USER CODE BEGIN EV */
ErrorEnum IRQ_Init(void)
{
	ErrorEnum err = ERROR_OK;

	adc1Channels = ADC_getAdcStruct(ADC_1_HANDLE);
	adc2Channels = ADC_getAdcStruct(ADC_2_HANDLE);

	if ((adc1Channels == NULL) || (adc2Channels == NULL)) {
		err = ERROR_NULL;
	}

	return err;
}

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  SchedulerCallback();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */
  CAN_RxCallback();
  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc2);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */
  Utils_RollingAverage_U16(&adc2Channels[ADC_CHANNEL_TPS_1].avgData, *adc2Channels[ADC_CHANNEL_TPS_1].raw);
  Utils_RollingAverage_U16(&adc2Channels[ADC_CHANNEL_TPS_2].avgData, *adc2Channels[ADC_CHANNEL_TPS_2].raw);
  Utils_RollingAverage_U16(&adc2Channels[ADC_CHANNEL_APPS_1].avgData, *adc2Channels[ADC_CHANNEL_APPS_1].raw);
  Utils_RollingAverage_U16(&adc2Channels[ADC_CHANNEL_APPS_2].avgData, *adc2Channels[ADC_CHANNEL_APPS_2].raw);

#if CONFIG_RUN_DEBUG
/* For STMSTUDIO */
  debugAdc2[ADC_CHANNEL_TPS_1] = *adc2Channels[ADC_CHANNEL_TPS_1].raw;
  debugAdc2[ADC_CHANNEL_TPS_2] = *adc2Channels[ADC_CHANNEL_TPS_2].raw;
  debugAdc2[ADC_CHANNEL_APPS_1] = *adc2Channels[ADC_CHANNEL_APPS_1].raw;
  debugAdc2[ADC_CHANNEL_APPS_2] = *adc2Channels[ADC_CHANNEL_APPS_2].raw;
#endif

#if CONFIG_ADC_SHOW_MIN_MAX
  if (HAL_GetTick() > 100U) {
	  Utils_UpdateMinMax_U16(adc2Channels[ADC_CHANNEL_TPS_1].avgData.avg, &adc2Channels[ADC_CHANNEL_TPS_1].min, &adc2Channels[ADC_CHANNEL_TPS_1].max);
	  Utils_UpdateMinMax_U16(adc2Channels[ADC_CHANNEL_TPS_2].avgData.avg, &adc2Channels[ADC_CHANNEL_TPS_2].min, &adc2Channels[ADC_CHANNEL_TPS_2].max);
	  Utils_UpdateMinMax_U16(adc2Channels[ADC_CHANNEL_APPS_1].avgData.avg, &adc2Channels[ADC_CHANNEL_APPS_1].min, &adc2Channels[ADC_CHANNEL_APPS_1].max);
	  Utils_UpdateMinMax_U16(adc2Channels[ADC_CHANNEL_APPS_2].avgData.avg, &adc2Channels[ADC_CHANNEL_APPS_2].min, &adc2Channels[ADC_CHANNEL_APPS_2].max);
  }
#endif

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream4 global interrupt.
  */
void DMA2_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream4_IRQn 0 */

  /* USER CODE END DMA2_Stream4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA2_Stream4_IRQn 1 */
  Utils_RollingAverage_U16(&adc1Channels[ADC_CHANNEL_GEAR_SENS].avgData, *adc1Channels[ADC_CHANNEL_GEAR_SENS].raw);
  //Utils_RollingAverage_U16(&adc1Channels[ADC_CHANNEL_IS].avgData, *adc1Channels[ADC_CHANNEL_IS].raw);

#if CONFIG_RUN_DEBUG
  /* For STMSTUDIO */
  debugAdc1[ADC_CHANNEL_GEAR_SENS] = *adc1Channels[ADC_CHANNEL_GEAR_SENS].raw;
  //debugAdc1[ADC_CHANNEL_IS] = *adc1Channels[ADC_CHANNEL_IS].raw;
#endif

  /* USER CODE END DMA2_Stream4_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

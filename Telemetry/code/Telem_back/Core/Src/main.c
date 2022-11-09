/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "can.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <abs.h>
#include <amorki.h>
#include <break_pressure.h>
#include <can_handler.h>
#include "PTT_handler.h"
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

/* USER CODE BEGIN PV */

// BREAK PRESSURE
float pressure[2]; //Pressure [bar]
uint16_t break_press[2]; //Analog value

// VSS
float wheel_speed[2];

//AMORKI
float displacement[2]; //move [mm]
uint16_t damper[2];	//Analog value

// PTT
int16_t transmission_check=3000; // is button still pressed

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  absInit(&absRRSensor, 0x01, &htim2, TIM_CHANNEL_1);
  absInit(&absLRSensor, 0x02, &htim3, TIM_CHANNEL_1);

  BPInit(&BP_Front_Sensor,0x01, &hadc1, ADC_CHANNEL_8);
  BPInit(&BP_Rear_Sensor,0x01,  &hadc1, ADC_CHANNEL_9);

  DampInit(&DampLRSensor, 0x01, &hadc2, ADC_CHANNEL_5);
  DampInit(&DampRRSensor, 0x01, &hadc2, ADC_CHANNEL_6);



  HAL_CAN_Start(&hcan1);
  CAN_Init(&hcan1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
   {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

 	  //LED STATUS
 	HAL_GPIO_WritePin(LED_Status_GPIO_Port, LED_Status_Pin, 1);


	/*
 	  //BREAK PREASSURE
 	BP_GetData(&hadc1,break_press);
 	for(int i=0;i<2;i++)
 	{
 	pressure[i]=BPCalculate(break_press[i]);
 	}

 	//AMORKI

 	Amorki_GetData(&hadc2, damper);
 	for(int i=0;i<2;i++)
 	{
 		displacement[i]=AmorkiCalculate(damper[i]);
 	}
 	*/


 	  //ABS
 	/*
 	if(flagL==0x01)
 	{
 		wheel_speed[0]=absCalculate(absLRSensor.data);
 		flagL=0x00;
 	}
 	if(flagR==0x01)
 	{
 		wheel_speed[1]=absCalculate(absRRSensor.data);
 		flagR=0x00;
 	}
 	*/
 	 //BP
 	BP_GetData(&BP_Front_Sensor);
 	BP_GetData(&BP_Rear_Sensor);

 	 //DAMPER
 	Amorki_GetData(&DampLRSensor);
	Amorki_GetData(&DampRRSensor);


 	//PTT
 	if(PTT_status)				//ACTIVE
 	{
 		HAL_GPIO_WritePin(PTT_GPIO_Port, PTT_Pin, 1);
 		//transmission_check=3000;
 	}
 	else						//DEACTIVE
 		HAL_GPIO_WritePin(PTT_GPIO_Port, PTT_Pin, 0);

 	//SENDING OVER CAN
 	/*
 	pressure[0]=1000;
 	pressure[1]=32.2;
 	displacement[0]=23.2;
 	displacement[1]=30;
 	wheel_speed[0]=2.56;
 	*/

 	BP_F_over_can(&BP_Front_Sensor);
 	HAL_Delay(1);
	BP_R_over_can(&BP_Rear_Sensor);
 	HAL_Delay(1);

 	Damper_LR_over_can(&DampLRSensor);
 	HAL_Delay(1);
 	Damper_RR_over_can(&DampRRSensor);
 	HAL_Delay(1);

 	VSS_LR_over_can(&absLRSensor);
 	HAL_Delay(1);
 	VSS_RR_over_can(&absRRSensor);
 /*

 	if(transmission_check<0)	//RESET
 	{
 		PTT_status=0;
 	}
 */


   }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

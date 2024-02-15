/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


#include "handler.h"
#include "sdcard/SDCARD.h"
#include "ecumaster.h"
#include "Utils/time.h"

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

 extern sensorDataHandler _dataHandler[];
 extern SensorStatus statusRegister;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim == &htim14 )
  {
	  IMUInnerSensor.timeToNextRead -= 5;
	 if(IMUInnerSensor.timeToNextRead <= 0){
		 if(IMUInnerSensor.dataReady){
			 IMUInnerSensor.dataReady=0;
			 _dataHandler[IMU].dataReady = 1;
		 }

		 IMUInnerSensor.timeToNextRead = 0;
	 }
	 damperLFSensor.timeToNextRead -= 5;
	 if(damperLFSensor.timeToNextRead <= 0){
		 _dataHandler[DAMPERRF].dataReady = 1;
		 _dataHandler[DAMPERLF].dataReady = 1;
		 _dataHandler[DAMPERRR].dataReady = 1;
		 _dataHandler[DAMPERLR].dataReady = 1;
		 _dataHandler[WHEEL].dataReady = 1;
		 damperLFSensor.timeToNextRead = 5;
	 }
	 damperLFSensor.sendDataByCanTime -= 5;
	 if( damperLFSensor.sendDataByCanTime<= 0){
		 adcSendDataByCan(&damperLFSensor);
		 adcSendDataByCan(&damperRFSensor);
		 adcSendDataByCan(&damperRRSensor);
		 adcSendDataByCan(&damperLRSensor);
		 damperLFSensor.sendDataByCanTime = 100;
	 }
	 absLFSensor.timeToNextRead -= 5;
	 if(absLFSensor.timeToNextRead <= 0)
	 {
		 ABSCalculate(&absLFSensor);
		 absLFSensor.timestamp = getSeconds();
		 absLFSensor.timeToNextRead = 5;
		 _dataHandler[ABSLF].dataReady = 1;
	 }
	 absRFSensor.timeToNextRead -= 5;
	 if(absRFSensor.timeToNextRead <= 0)
	 {
		 ABSCalculate(&absRFSensor);
		 absRFSensor.timestamp = getSeconds();
		 absRFSensor.timeToNextRead = 5;
		 _dataHandler[ABSRF].dataReady = 1;
	 }

  }
}

//ABS Callback handler
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	ABSCallbackHandler(htim);
}
//GPS Callback handler
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart==gpsSensor.uart)
	{
		GPSCallbackHandler();
	}
}

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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_SDMMC2_SD_Init();
  MX_I2C1_Init();
  MX_FATFS_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_I2C4_Init();
  MX_SPI4_Init();
  MX_UART7_Init();
  MX_USART1_UART_Init();
  MX_USB_OTG_FS_HCD_Init();
  MX_TIM2_Init();
  MX_TIM14_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  //initialise_monitor_handles();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  statusRegister.checkTime = SENSOR_ALL_CHECK_TIME;
  initSensors();

  HAL_TIM_Base_Start_IT(&htim14);


  FATFS fileSystem;
  sdInit(&fileSystem);
  printStatusRegister();
  if((statusRegister.SDCARD & 0b100) < SENSOR_FAIL){
  	  openAllFiles();
  }

  {
	  char time[25];
	  getTimeInUTCFormat(time);
	  printf("Current Date: %02d/%02d/%04d\n", globalDate.Date, globalDate.Month,2000 + globalDate.Year);
	  printf("Current time: %s\n", time);
  }
  //ALL INITIALIZED WITHOUT FATAL ERROR TURN LED ON
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);

  int timePassed = HAL_GetTick();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    for(SENSORS i=0;i<SENSORS_N;i++)
  	{
  		if( _dataHandler[i].isActive)
  		{
  			if(_dataHandler[i].getDataHandler != NULL)
  			{
  				 _dataHandler[i].getDataHandler(_dataHandler[i].sensorStruct);
  			}
  			if((statusRegister.SDCARD & 0b100) < SENSOR_FAIL)
  			{
				if(_dataHandler[i].dataReady == 1 && _dataHandler[i].saveDataHandler != NULL)
				{
					_dataHandler[i].saveDataHandler(_dataHandler[i].sensorStruct);
					_dataHandler[i].dataReady = 0;
				}
  			}else
  			{
				if(HAL_GetTick() - timePassed > 1000)
				{
					HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3);
					timePassed = HAL_GetTick();
					printf("SD card not initialized\n");
				}

  			}
  		}
	}
	//sendEcuLogs(ecuData);
    if((statusRegister.SDCARD & 0b100) < SENSOR_FAIL)
	{
    	if(HAL_GetTick() - timePassed > 5000)
  		{
			sdFlush();
			timePassed = HAL_GetTick();
		}
	}

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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
	  for(int i =1000000;i > 0; i--){
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3);
	  }
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

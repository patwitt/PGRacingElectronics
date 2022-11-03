/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "basicFunctions.h"
#include "handler.h"
#include "sdcard/SDCARD.h"
#include "ecumaster.h"
#include "logger/transmitter.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */




/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TRUE 1
#define FALSE 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern EcumasterData EcuData;
extern sensorDataHandler _dataHandler[];
extern SensorStatus statusRegister;
extern ADCSensor damperRFSensor;
extern ADCSensor damperLFSensor;
extern FIL* EcuFile;
extern FIL* StatsFile;
extern char ecuPath[];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
FATFS fileSystem;

int sendFileToUart(FIL * f,char * path)
{
	 FRESULT res;
	 UINT dmy;
	 char buff[1000];
	 f_close(f);
	 res = f_open(f, path, FA_READ);
	 if (res) return res;
	 HAL_Delay(1000);
	 sprintf(buff,"%s\n", path);
	 HAL_UART_Transmit(&huart7,  buff, strlen(buff), HAL_MAX_DELAY);
	 //HAL_UART_Transmit(&huart7,  '\n', 1, HAL_MAX_DELAY);
	 HAL_Delay(1000);
	 while (res == FR_OK && !f_eof(f)) {

	        res = f_read(f, buff, 1000, &dmy);
	        HAL_UART_Transmit(&huart7, buff, dmy,HAL_MAX_DELAY);
	 }
	 HAL_Delay(1000);
	 HAL_UART_Transmit(&huart7, "\n", 1, HAL_MAX_DELAY);
	 HAL_UART_Transmit(&huart7, "EOF\n", 4, HAL_MAX_DELAY);
	 f_close(f);
	 return res;
}
int sendAllFilesToUart()
{
	FILINFO f;

	HAL_UART_Transmit(&huart7, "TORBA", 5, HAL_MAX_DELAY);
	HAL_Delay(1000);
	if(f_stat(absLFSensor.path, &f)==FR_OK)
		{
			sendFileToUart(absLFSensor.File, absLFSensor.path);
		}
	if(f_stat(mlxLFSensor.path, &f)==FR_OK)
	{
		sendFileToUart(mlxLFSensor.File, mlxLFSensor.path);
	}
	if(f_stat(gyro.path, &f)==FR_OK)
	{
		sendFileToUart(gyro.File, gyro.path);
	}
	if(f_stat(gpsSensor.path, &f)==FR_OK)
	{
		sendFileToUart(gpsSensor.File, gpsSensor.path);
	}
	if(f_stat(ecuPath, &f)==FR_OK)
	{
		sendFileToUart(EcuFile, ecuPath);
	}
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart7, "BORBA\n", 6, HAL_MAX_DELAY);
	return 0;

}
int measureTime(uint32_t startTime){
	return HAL_GetTick()- startTime;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim == &htim14 )
  {

	 statusRegister.checkTime -= 25;
	 if( statusRegister.checkTime <= 0)
	 {
		 //Check all sensors
		 statusRegister.checkTime = SENSOR_ALL_CHECK_TIME;
		 printStatusRegister();

	 }
	 gyro.timeToNextRead -= 25;
		 if(gyro.timeToNextRead <= 0){
			 _dataHandler[GYRO].dataReady = 1;
			 gyro.timeToNextRead = 250;
		 }
	 damperLFSensor.timeToNextRead -= 25;
	 if(damperLFSensor.timeToNextRead <= 0){
		 _dataHandler[DAMPERRF].dataReady = 1;
		 _dataHandler[DAMPERLF].dataReady = 1;
		 damperLFSensor.timeToNextRead = 50;
	 }
	 absLFSensor.timeToZeroSpeed -= 25;
	 if(absLFSensor.timeToZeroSpeed <= 0)
	 {
		 _dataHandler[ABSLF].dataReady = 1;
		 _dataHandler[ABSRF].dataReady = 1;
		 absLFSensor.timeToZeroSpeed = 50;
	 }
  }
}
int getTime(RTC_TimeTypeDef* time, RTC_DateTypeDef* date)
{
	HAL_RTC_GetTime(&hrtc, time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, date, RTC_FORMAT_BIN);
    return ((time->SecondFraction-time->SubSeconds)/((float)time->SecondFraction+1) * 1000);
}


//ABS
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	ABSCallbackHandler(htim);
}
char received_command[255];
int command_lenght = 0;
uint8_t bufor;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart==gpsSensor.uart)
	{
		GPSCallbackHandler();

	}else if(huart ==&huart3)
	{
		received_command[command_lenght] = bufor;
		command_lenght++;
		if(bufor == '\n'){
			received_command[command_lenght] = 0;
			parseCommand(received_command);
			command_lenght = 0;

		}
		HAL_UART_Receive_IT(&huart3, &(bufor), 1);
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
  MX_USART3_UART_Init();
  MX_FATFS_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_UART7_Init();
  MX_SDMMC2_SD_Init();
  MX_RTC_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_I2C3_Init();
  MX_I2C4_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */

  statusRegister.checkTime = SENSOR_ALL_CHECK_TIME;
  initSensors();

  HAL_TIM_Base_Start_IT(&htim14);
  HAL_UART_Receive_IT(&huart3, &(bufor), 1);

  sdInit(&fileSystem);
  printStatusRegister();
  if((statusRegister.SDCARD & 0b100) < SENSOR_FAIL){
	  openAllFiles();
  }

  HAL_Delay(200);
/*
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
  printf("Aktualny czas: %02d:%02d:%02d\n", time.Hours, time.Minutes, time.Seconds);
*/



  //

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if((statusRegister.SDCARD & 0b100) < SENSOR_FAIL)
	  {
		  int startTime = 0;
		  for(SENSORS i=0;i<SENSORS_N;i++)
		  {
			  if( _dataHandler[i].isActive)
			  {
				  if(_dataHandler[i].getDataHandler != NULL)
				  {
					  startTime = HAL_GetTick();
					   _dataHandler[i].getDataHandler(_dataHandler[i].sensorStruct);
					  statsSave(0, startTime, i);

				  }
				  if(_dataHandler[i].dataReady == 1 && _dataHandler[i].saveDataHandler != NULL)
				  {
					  startTime = HAL_GetTick();
					  _dataHandler[i].saveDataHandler(_dataHandler[i].sensorStruct);
					  _dataHandler[i].dataReady = 0;
					  statsSave(1, startTime, i);
				  }

			  }
		  }
		  startTime = HAL_GetTick();
		  sendEcuLogs(EcuData);
		  sdFlush();
		  statsSave(2, startTime, 9);

		  //Add sd flush here to decrease number of syncing files
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_UART7|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_I2C3
                              |RCC_PERIPHCLK_I2C4|RCC_PERIPHCLK_SDMMC2
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart7ClockSelection = RCC_UART7CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc2ClockSelection = RCC_SDMMC2CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

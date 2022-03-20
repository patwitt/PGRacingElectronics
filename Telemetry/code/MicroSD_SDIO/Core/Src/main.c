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
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "MPU9250.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return 1;
}

struct imu_9dof
{
	int16_t acc_data[3];
	int16_t mag_data[3];
	int16_t gyro_data[3];
};

struct imu_9dof_calc
{
	double acc_data_calc[3];
	double gyro_data_calc[3];
};

void imu_9dof_convert(struct imu_9dof * input, struct imu_9dof_calc * output)
{
	output->acc_data_calc[0] = (double) input->acc_data[0] / 16384;
	output->acc_data_calc[1] = (double) input->acc_data[1] / 16384;
	output->acc_data_calc[2] = (double) input->acc_data[2] / 16384;
	output->gyro_data_calc[0] = (double) input->gyro_data[0] * 250 / 32768;
	output->gyro_data_calc[1] = (double) input->gyro_data[1] * 250 / 32768;
	output->gyro_data_calc[2] = (double) input->gyro_data[2] * 250 / 32768;
}

void imu_9dof_get_data(struct imu_9dof * imu_9dof_data, struct imu_9dof_calc * imu_9dof_calculated)
{
    MPU9250_GetData(imu_9dof_data->acc_data, imu_9dof_data->mag_data, imu_9dof_data->gyro_data);
    imu_9dof_convert(imu_9dof_data, imu_9dof_calculated);
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
  MX_SDMMC1_SD_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  FATFS temp_fatfs;
  FIL fil;;
  UINT test_byte;

  /*HAL_SD_CardInfoTypeDef SDCardInfo1;

  HAL_SD_Get_CardInfo(&hsd2, &SDCardInfo1);


  printf('SD capacity: %d\n',SDCardInfo1.BlockNbr);
  //HAL_UART_Transmit(&huart3, "no filesystem\r\n", 6, HAL_MAX_DELAY);
      //UG_ConsolePutString(''uSD: HAL_SD_Get_CardInfo != SD_OK!\n'');


*/
  struct imu_9dof imu_9dof_data;
  struct imu_9dof_calc imu_9dof_calculated;
  char testdata[255] ;
FRESULT fres = 0;

  HAL_Delay(1000);

  if (f_mount(&temp_fatfs, "", 1) == FR_OK)
  {
	  char path[] = "GYRO.TXT\0";
	 fres = f_open(&fil, path, FA_WRITE | FA_CREATE_ALWAYS);

	  char test_data[] = "number,gyro_x,gyro_y,gyro_z,acc_x,acc_y,acc_z\r\n";
	  fres =  f_write(&fil, test_data, sizeof(test_data), &test_byte);
	  fres =   f_close(&fil);
	  //HAL_UART_Transmit(&huart3, "sIEMA\r\n", 7, HAL_MAX_DELAY);
  }
  //HAL_UART_Transmit(&huart3, "no filesystem\r\n", 6, HAL_MAX_DELAY);
  int number = 1;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  char path[] = "GYRO.TXT\0";
	sprintf(testdata,"%d,,", number);
	  f_open(&fil, path, FA_WRITE | FA_OPEN_APPEND);
	imu_9dof_get_data(&imu_9dof_data, &imu_9dof_calculated);
	for (int i = 0; i < 3; i++)
	{
		sprintf(testdata, "%f,", imu_9dof_calculated.gyro_data_calc[i]);
		  f_write(&fil, testdata, strlen(testdata), &test_byte);
		printf("%f ", imu_9dof_calculated.gyro_data_calc[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		sprintf(testdata, "%f,", imu_9dof_calculated.gyro_data_calc[i]);
		  f_write(&fil, testdata, strlen(testdata), &test_byte);
		printf("%f ", imu_9dof_calculated.acc_data_calc[i]);
	}
	sprintf(testdata, "\r\n ");
	number++;
	f_write(&fil, testdata, strlen(testdata), &test_byte);
	printf("\r\n");






	  f_close(&fil);

	  HAL_Delay(1000);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV8;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_SDMMC1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_SYSCLK;
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
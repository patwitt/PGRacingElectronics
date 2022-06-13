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
#include "basicFunctions.h"
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33
#define	 RefreshRate 0x04
#define  TA_SHIFT 8 //Default shift for MLX90640 in open air

static uint16_t eeMLX90640[832];

uint16_t frame[834];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FILE_DEFAULT_WRITE FA_WRITE | FA_OPEN_APPEND
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
typedef struct MLX{
	FIL mlxFile;
	I2C_HandleTypeDef i2c;
	int dataReady;
	paramsMLX90640 mlx90640;
	float data[768];
	float ambientTemp;
	float vdd;
	float emissivity;

}MLX;
MLX mlx;
int mlxInit()
{
		static uint16_t eeMLX90640[832];
		HAL_Delay(200);
		MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate);
	  	MLX90640_SetChessMode(MLX90640_ADDR);
	  	mlx.emissivity = 0.95;
	    int status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640);
	    if (status != 0) return status;
	    status = MLX90640_ExtractParameters(eeMLX90640, &mlx.mlx90640);
	    if (status != 0) return status;

	    return 0;

}
int mlxGetData(){
	int status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
	if (status < 0)
	{
		printf("Error!\n");
		return status;
	}

	mlx.vdd = MLX90640_GetVdd(frame, &mlx.mlx90640);
	mlx.ambientTemp = MLX90640_GetTa(frame, &mlx.mlx90640) - TA_SHIFT;

	MLX90640_CalculateTo(frame, &mlx.mlx90640, mlx.emissivity , mlx.ambientTemp, mlx.data);
	status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
			if(status < 0)
			{
				printf("Error!\n");
			}
	MLX90640_CalculateTo(frame, &mlx.mlx90640, mlx.emissivity , mlx.ambientTemp, mlx.data);
	return 0;
}
void mlxPrintData()
{
	for(int i = 0; i < 768; i++){
		if(i%32 == 0 && i != 0){
			printf("\r\n");
		}
		printf("%2.2f ",mlx.data[i]);
	}
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
int createHeader(FIL * file,char * path)
{
	FRESULT fres;
	int bytesWritten = 0;
	fres = f_write(file,"timestamp,",strlen("timestamp,"),&bytesWritten);
	if(fres != FR_OK){
		printf("Error while creating %s header",path);
		return -1;
	}
	if(strcmp(path,"GYRO.csv") == 0){
		fres = f_write(file, "number,gyro_x,gyro_y,gyro_z,acc_x,acc_y,acc_z\r\n", strlen("timestamp,number,gyro_x,gyro_y,gyro_z,acc_x,acc_y,acc_z\r\n"), &bytesWritten);
		if(fres != FR_OK)
		{
			printf("Error while creating %s header\n",path);
			return -1;
		}
	}else if(strcmp(path,"MLX.csv") == 0){
		char headerData[25];

		for(int i=0;i<784;i++){
			sprintf(headerData,"float_%d,", i);
			fres =  f_write(file, headerData, strlen(headerData), &bytesWritten);
			if(fres != FR_OK){
				printf("Error while creating %s header\n",path);
				return -1;
			}
		}

	}else
	{
		return -2;
	}
	fres =  f_write(file, "\n", strlen("\n"), &bytesWritten);
	return 1;

}

void openFile(FIL * file, char * path, BYTE mode)
{
	FILINFO fInfo;
	FRESULT fres = f_stat(path, &fInfo);
	if(fres == FR_OK)
	{
		fres = f_open(file, path, mode);
		if(fres == FR_OK)
		{
			printf("Opening file: %s succeeded\n", path);
		}
	}else if(fres == FR_NO_FILE)
	{
		fres = f_open(file, path, mode);
		if(fres == FR_OK)
		{
			createHeader(file,path);
			printf("No file: %s, created new\n", path);
		}
	}
	f_sync(file);


}
void mountFailHandler()
{
	printf("SDCard mount failed\n");
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
  int res = mlxInit();
  if(res != 0)
  {
    printf("Error while initializing mlx!\n");
  }
  FATFS fileSystem;
  FIL gyroFIL;
  FIL MLXFIL;

  UINT writedBytes;
  struct imu_9dof imu_9dof_data;
  struct imu_9dof_calc imu_9dof_calculated;
  MPU9250_Init();
  char testdata[255];
  FRESULT fres = 0;

  HAL_Delay(1000);

  if (f_mount(&fileSystem, "", 1) == FR_OK)
  {
	  printf("SDCard mounting success!\n");
	  openFile(&MLXFIL, "MLX.csv", FILE_DEFAULT_WRITE);
	  openFile(&gyroFIL, "GYRO.csv", FILE_DEFAULT_WRITE);

  }else
  {
	  mountFailHandler();
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
	  char path[] = "GYRO.csv\0";
	  char path2[] = "MLX.csv\0";

	  sprintf(testdata, "%d,", HAL_GetTick());
	f_write(&gyroFIL, testdata, strlen(testdata), &writedBytes);
	f_write(&MLXFIL, testdata, strlen(testdata), &writedBytes);

	imu_9dof_get_data(&imu_9dof_data, &imu_9dof_calculated);
	for (int i = 0; i < 3; i++)
	{
		sprintf(testdata, "%f,", imu_9dof_calculated.gyro_data_calc[i]);
		  f_write(&gyroFIL, testdata, strlen(testdata), &writedBytes);
		printf("%f ", imu_9dof_calculated.gyro_data_calc[i]);

	}
	for (int i = 0; i < 3; i++)
	{
		sprintf(testdata, "%f,", imu_9dof_calculated.gyro_data_calc[i]);
		  f_write(&gyroFIL, testdata, strlen(testdata), &writedBytes);
		printf("%f ", imu_9dof_calculated.acc_data_calc[i]);
	}
	sprintf(testdata, "\r\n ");
	number++;
	f_write(&gyroFIL, testdata, strlen(testdata), &writedBytes);
	printf("\r\n");
	mlxGetData();
	printf("start = %d \r\n", HAL_GetTick());
	for(int i=0;i<784;i++)
		  {
			  sprintf(testdata,"%2.2f,", mlx.data[i]);
			  fres =  f_write(&MLXFIL, testdata, strlen(testdata), &writedBytes);
		  }
	printf("end = %d \r\n", HAL_GetTick());

	sprintf(testdata, "\r\n ");
		f_write(&MLXFIL, testdata, strlen(testdata), &writedBytes);
		f_sync(&gyroFIL);
	  f_sync(&MLXFIL);
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

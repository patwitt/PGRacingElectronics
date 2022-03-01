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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return 1;
}


#define LCD_ADDRESS 0x78

uint8_t commands[4] = {0x00, 0x00};

HAL_StatusTypeDef halstatus = 0;


uint8_t customchar1[9] = {
		0x40,
		0b10000,
		0b11000,
		0b11100,
		0b11111,
		0b11111,
		0b11100,
		0b11000,
		0b10000,
};


uint8_t customchar2[9] = {
		0x40,
		0b00000,
		0b01010,
		0b00000,
		0b10001,
		0b01110,
		0b00000,
		0b00000,
		0b00000,
};

uint8_t oneplacer[9] = {
		0x40,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000
};

uint8_t Helper_Array[8][5];

void Init_LCD()

{

    HAL_GPIO_WritePin(GPIOA, RESET_Pin, GPIO_PIN_SET); // make LCD_RST high to start LCD module

    HAL_Delay(60);

    halstatus = HAL_I2C_IsDeviceReady(&hi2c1, LCD_ADDRESS, 2, 100);

    commands[1] = 0x38;

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);

    commands[1] = 0x39;

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);

    commands[1] = 0x14;

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);

    commands[1] = 0x78;

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);

    commands[1] = 0x5e;

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);

    commands[1] = 0x6d;

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);

    commands[1] = 0x0c;

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);

    commands[1] = 0x01;

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);

    commands[1] = 0x06;

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);
}

void Show_LCD(uint8_t text_string[])

{
    commands[1] = 0x80; // First character position of first Line

    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

    HAL_Delay(60);

    uint8_t* data = (uint8_t*)malloc(strlen(text_string) + 1);
    data[0] = 0x40;

    for(uint8_t i = 1; i <= strlen(text_string) + 1; i++){
    	data[i] = text_string[i - 1];
    }
    halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, data, strlen(text_string) + 1, 100);

    HAL_Delay(60);
}



void Update_Char()
{
	HAL_Delay(5);

	commands[1] = 0x38;

	halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);
	HAL_Delay(5);

	commands[1] = 0x48;
	halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);

	HAL_Delay(5);

	halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, oneplacer, 9, 100);
	HAL_Delay(5);

	commands[1] = 0x39; // First character position of first Line
	halstatus = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, commands, 2, 100);
	HAL_Delay(5);
}


void ClearHelperArray()
{
	for(uint8_t i=0; i < 8; i++){
			for(uint8_t j=0; j < 5; j++)
			{
				Helper_Array[i][j] = 0;
			}
	}
}

void HelperToBytes()
{
	for(uint8_t i=0,k=1; i < 8; i++, k++){
			uint8_t value = 0;
			for(uint8_t j=0; j < 5; j++)
			{

				if(Helper_Array[i][j] == 1)
				{
					value += pow(2,5-j);
				}
			}
			oneplacer[k] = value;
		}
}


void Snake()
{
	for(uint8_t i=0; i < 8; i++){
		for(uint8_t j=0; j < 5; j++)
		{
			ClearHelperArray();
			Helper_Array[i][j] = 1;
			HelperToBytes();
			Update_Char();
			HAL_Delay(100);
		}
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
  MX_I2C1_Init();


  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
  Init_LCD();


  Show_LCD("\001\001\001\001\001\001\001\001\001\001\001");
  HAL_Delay(1000);
  /*
  uint8_t text_string[20] = "         ";

  Show_LCD("                  ");
  for(int i=0; i < 20; i++ )
  {
	  text_string[i] = '\001';
	  Show_LCD(text_string);
	  HAL_Delay(200);
  }
  */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	 Snake();
	 HAL_Delay(500);
    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00303D5B;
  hi2c1.Init.OwnAddress1 = 20;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RESET_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : RESET_Pin */
  GPIO_InitStruct.Pin = RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

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

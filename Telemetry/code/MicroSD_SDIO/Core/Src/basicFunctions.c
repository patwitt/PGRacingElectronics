/*
 * basicFunctions.c
 *
 *  Created on: May 27, 2022
 *      Author: Patryk
 */

#ifndef BASICFUNCTIONS_H_
#define BASICFUNCTIONS_H_
#define defaultI2C hi2c2
#define uartDef huart3
/* Includes ------------------------------------------------------------------*/
#include "basicFunctions.h"
#include "stdint.h"
#include "stm32f7xx_hal.h"
#include <string.h>
extern UART_HandleTypeDef uartDef;
extern I2C_HandleTypeDef defaultI2C;

void I2CScanner()
{
	uint8_t Buffer[25] = {0};
	uint8_t Space[] = " - ";
	uint8_t StartMSG[] = "Starting I2C Scanning: \r\n";
	uint8_t EndMSG[] = "Done! \r\n\r\n";
	HAL_UART_Transmit(&uartDef, StartMSG, sizeof(StartMSG), 10000);
	for(int i=67; i<128	; i++){
			int ret = HAL_I2C_IsDeviceReady(&defaultI2C, (uint16_t)(i<<1), 3, 5);
			if (ret != HAL_OK) /* No ACK Received At That Address */
			{
				HAL_UART_Transmit(&uartDef, Space, sizeof(Space), 10000);
			}
			else if(ret == HAL_OK)
			{
				sprintf(Buffer, "0x%X", i);
				HAL_UART_Transmit(&uartDef, Buffer, sizeof(Buffer), 10000);
			}
	}
		HAL_UART_Transmit(&uartDef, EndMSG, sizeof(EndMSG), 10000);
};
int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		__io_putchar(*ptr++);
	}
	return len;
}
int __io_putchar(int ch){
	if(ch == '\n')
	{
		HAL_UART_Transmit(&uartDef, (uint8_t*)"\r", 1, HAL_MAX_DELAY);
	}
	HAL_UART_Transmit(&uartDef, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return 1;
}
#endif /* BASICFUNCTIONS_H_ */

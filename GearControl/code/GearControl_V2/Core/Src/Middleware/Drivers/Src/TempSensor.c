#include "TempSensor.h"

typedef struct {
	TempSensStatus status;
	float temp;
} TempSensorHandler;

static TempSensorHandler sens_;

int DS18B20_Start(void)
{
	uint8_t data = 0xF0;
	uart_init(9600);
	HAL_UART_Transmit(&huart1, &data, 1, 100);  // low for 500+ms
	if (HAL_UART_Receive(&huart1, &data, 1, 1000) != HAL_OK) return SENS_RECV_FAIL;   // failed.. check connection
	uart_init(115200);
	if (data == 0xF0) return SENS_NO_RESPONSE;  // no response.. check connection
	return SENS_OK;  // response detected
}


void DS18B20_Write(const uint8_t data)
{
  static uint8_t buffer[8];

  for (uint32_t i = 0U; i < 8U; ++i){
    if ((data & (1U << i)) != 0U) {
    	buffer[i] = 0xFFU;  // write 1
    } else  {
    	buffer[i] = 0U;  // write 0
    }
  }

  HAL_UART_Transmit(&huart1, buffer, 8, 100);
}

uint8_t DS18B20_Read(void)
{
	uint8_t buffer[8];
	uint8_t value = 0;
	for (int i=0; i<8; i++)
	{
		buffer[i] = 0xFF;
	}

	HAL_UART_Transmit_DMA(&huart1, buffer, 8);
	HAL_UART_Receive_DMA(&huart1, RxData, 8);

	while (isRxed == 0);

	for (uint32_t i = 0U; i < 8U; ++i)
	{
		if (RxData[i] == 0xFF)  // if the pin is HIGH
		{
			value |= 1 << i;  // read = 1
		}
	}
	isRxed = 0;
	return value;
}

void TempSensor_Task(void)
{
	sens_.status = DS18B20_Start();

	if (sens_.status == SENS_OK) {
		 DS18B20_Write (0xCC);  // skip ROM
		 DS18B20_Write (0x44);  // convert t

		 sens_.status = DS18B20_Start();
		 if (sens_.status == SENS_OK) {
			 DS18B20_Write (0xCC);  // skip ROM
			 DS18B20_Write (0xBE);  // Read Scratch-pad

			 const uint8_t temp_LSB = DS18B20_Read();
			 const uint8_t temp_MSB = DS18B20_Read();

			 sens_.temp = (float)(((temp_LSB << 8U)) | temp_MSB) / 16.0;  // resolution is 0.0625
		 }
	}
}

TempSensorStatus TempSensor_GetStatus(void)
{
	return sens_.status;
}

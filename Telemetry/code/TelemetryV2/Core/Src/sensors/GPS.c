/*
 * GPS.c
 *
 *  Created on: Aug 15, 2022
 *      Author: Patryk
 */
#include <stdlib.h>
#include "sensors/GPS.h"
#include "string.h"
#include "handler.h"


extern UART_HandleTypeDef huart3;
extern RTC_HandleTypeDef hrtc;
extern sensorDataHandler _dataHandler[];
extern SensorStatus statusRegister;
extern GPSSensor gpsSensor;
typedef struct{
	float lat;
	float lon;
}Coords;
char** splitCSV(char * data)
{
	int i=0;
	int j=0;
	int k = 0;
	char **array = (char**)malloc(sizeof(char*)*13);
	while(data[i]!='\0' || k<13){

		char * column = (char*)malloc(15);
		for(j = 0;data[i]!=','&&data[i]!='\0';i++,j++)
		{
			column[j]=data[i];
		}
		i++;
		column[j]='\0';
		array[k]=column;
		k++;
	}


	return array;
}
Coords getCoords(char**array){
	Coords coord = {};
	if(strcmp(array[0], "$GPRMC") == 0){
		coord.lat = atof(array[3]);
		if(strcmp(array[4], "S") == 0){
			coord.lat *= -1;
		}
			coord.lon = atof(array[5]);
		}
	return coord;
}

int isDataValid(char**array){
	if(strcmp(array[0], "$GPRMC") == 0){
		if(strcmp(array[3],"A") == 0){
			return 1;
		}else{
			return 0;
		}
	}
	return -1;
}

void GPSReinitUart(GPSSensor * sens)
{
  HAL_UART_Abort_IT(sens->uart);
  HAL_UART_DeInit(sens->uart);
  sens->uart->Instance = USART3;
  sens->uart->Init.BaudRate = 115200;
  sens->uart->Init.WordLength = UART_WORDLENGTH_8B;
  sens->uart->Init.StopBits = UART_STOPBITS_1;
  sens->uart->Init.Parity = UART_PARITY_NONE;
  sens->uart->Init.Mode = UART_MODE_TX_RX;
  sens->uart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  sens->uart->Init.OverSampling = UART_OVERSAMPLING_16;
  sens->uart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  sens->uart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  sens->uart->AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_UART_Init(sens->uart) != HAL_OK)
  {
	  Error_Handler();
  }
}


void GPSInit(GPSSensor * sens)
{
	sens->File = (FIL*)malloc(sizeof(FIL));
	sens->saveRate = GPS_ERROR_TIME;
	sens->uart = &huart3;
	sens->saveLock = 0;
	sens->buforSize = 0;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"GPS%02d%02d.csv",date.Date,date.Month);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, 1);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, 1);
    HAL_Delay(1000);
    GPSChangeBaudrate();
    HAL_Delay(1000);
    GPSReinitUart(sens);
    HAL_UART_Receive_IT(sens->uart, &(sens->Rx_data), 1); // aktywacja przerwan
    GPSSetDataRate();
    HAL_Delay(1000);
    GPSDisableNmea();
    HAL_Delay(100);


}

void GPSSetDataRate(){
	char packet[] = {
        0xB5, // sync char 1
        0x62, // sync char 2
        0x06, // class
        0x08, // id
        0x06, // length
        0x00, // length
        0x28, // payload
        0x00, // payload
        0x01, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0x00, // CK_A
        0x00, // CK_B
    };
    char packetSize = sizeof(packet);
    for (char j = 0; j < packetSize - 4; j++)
	{
    	packet[packetSize - 2] += packet[2 + j];
    	packet[packetSize - 1] += packet[packetSize - 2];
    }
	HAL_UART_Transmit(&huart3, packet, packetSize, 10);
}
// Send a set of packets to the GPS receiver to disable NMEA messages.
void GPSDisableNmea()
{
    // An array of two bytes for CFG-MSG packets payload.
    char messages[][3] = {
        {0xF0, 0x0A}, //DTM
        {0xF0, 0x09}, //GBS
        {0xF0, 0x00}, //GGA
        {0xF0, 0x01}, //GLL
        {0xF0, 0x0D}, //GNS
        {0xF0, 0x06}, //GRS
        {0xF0, 0x02}, // GSA
        {0xF0, 0x07}, //GST
        {0xF0, 0x03}, // GSV
        {0xF0, 0x04, 0x01}, // RMC
        {0xF0, 0x0E}, //THS
        {0xF0, 0x0F},//VLW
        {0xF0, 0x05},//VTG
        {0xF0, 0x08},//ZDA
        {0xF1, 0x00},//POS
        {0xF1, 0x01},
        {0xF1, 0x03},
        {0xF1, 0x04},
        {0xF1, 0x05},
        {0xF1, 0x06},
    };

    // CFG-MSG packet buffer.
    char packet[] = {
        0xB5, // sync char 1
        0x62, // sync char 2
        0x06, // class
        0x01, // id
        0x03, // length
        0x00, // length
        0xF0, // payload (first byte from messages array element)
        0x04, // payload (second byte from messages array element)
        0x10, // payload (not changed in this case)
        0x00, // CK_A
        0x00, // CK_B
    };
    char packetSize = sizeof(packet);

    // Offset to a place where payload starts.
    char payloadOffset = 6;

    // Iterate over the messages array.
    for (char i = 0; i < sizeof(messages) / sizeof(*messages); i++)
    {
        // Copy two bytes of payload to the packet buffer.
        for (char j = 0; j < sizeof(*messages); j++)
        {
            packet[payloadOffset + j] = messages[i][j];
        }

        // Set checksum bytes to null.
        packet[packetSize - 2] = 0x00;
        packet[packetSize - 1] = 0x00;

        // Calculate checksum over the packet buffer excluding sync (first two) and checksum chars (last two).
        for (char j = 0; j < packetSize - 4; j++)
        {
            packet[packetSize - 2] += packet[2 + j];
            packet[packetSize - 1] += packet[packetSize - 2];
        }
        HAL_UART_Transmit(&huart3, packet, packetSize, 10);

    }
}
void GPSChangeBaudrate()
{
    // CFG-PRT packet.
    char packet[] = {
        0xB5, // sync char 1
        0x62, // sync char 2
        0x06, // class
        0x00, // id
        0x14, // length
        0x00, // length
        0x01, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0xD0, // payload
        0x08, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0xC2, // payload
        0x01, // payload
        0x00, // payload
        0x07, // payload
        0x00, // payload
        0x03, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0x00, // CK_A
        0x00, // CK_B
    };
    char packetSize = sizeof(packet);

    for (char j = 0; j < packetSize - 4; j++)
    {
        packet[packetSize - 2] += packet[2 + j];
        packet[packetSize - 1] += packet[packetSize - 2];
    }
    char bucker[40];
    HAL_UART_Transmit(&huart3, packet, sizeof(packet), 10);
    HAL_UART_Receive(&huart3, bucker, 30, 20);
}

void GPSCallbackHandler(){

	gpsSensor.bufor[gpsSensor.buforSize] = gpsSensor.Rx_data;
	gpsSensor.buforSize++;

	  if(gpsSensor.Rx_data == '\n' && gpsSensor.buforSize > 10)
	  {
		_dataHandler[GPS].dataReady = 0;
		gpsSensor.bufor[gpsSensor.buforSize]= 0;
		strcpy(gpsSensor.data,gpsSensor.bufor);
		gpsSensor.bufor[0]= '\0';
		gpsSensor.buforSize = 0;
		gpsSensor.Rx_data = '\0';
		gpsSensor.dataReady = 1;
		_dataHandler[GPS].dataReady = 1;
		gpsSensor.timestamp = getSeconds();

	  }
	  if(gpsSensor.buforSize > 255)
	  {
		 gpsSensor.bufor[0] = '\0';
		 gpsSensor.buforSize = 0;
		 _dataHandler[GPS].dataReady = 0;
	  }

	  HAL_UART_Receive_IT(gpsSensor.uart, &(gpsSensor.Rx_data), 1);
}



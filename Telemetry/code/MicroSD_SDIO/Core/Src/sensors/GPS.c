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
extern UART_HandleTypeDef huart7;
extern RTC_HandleTypeDef hrtc;
extern sensorDataHandler _dataHandler[];
extern SensorStatus statusRegister;
GPSSensor gpsSensor;
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


//GPS
char commands[4][255] = {"$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n",//Send Data setup
		"$PMTK251,115200*1f\r\n", //BuadRate change
		"$PMTK869,1,0*34\r\n", // Easy mode off
		"$PMTK220,100*2f\r\n"}; // Update Rate



int calculateCSM(char * command)
{
	int chsm = 0;
	char c;
	for(int i=0; i < strlen(command); i++)
	{
		c = command[i];
		chsm = chsm ^ c;
	}
	return chsm;
}

void createCommand(char * command)
{
	char final[255];
	int chsm = calculateCSM(command);
	sprintf(final, "$%s*%02X\r\n", command, chsm);
	strcpy(command,final);
}

void GPSReinitUart(GPSSensor * sens)
{
  HAL_UART_Abort_IT(sens->uart);
  HAL_UART_DeInit(sens->uart);
  sens->uart->Instance = UART7;
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
	sens->uart = &huart7;
	sens->saveLock = 0;
	sens->buforSize = 0;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"GPS%02d%02d.csv",date.Date,date.Month);
	HAL_UART_Transmit(sens->uart, commands[1], strlen(commands[1]), HAL_MAX_DELAY); // zmiana baudrate GPSa
	GPSReinitUart(sens);
	HAL_UART_Receive_IT(sens->uart, &(sens->Rx_data), 1); // aktywacja przerwan
	HAL_Delay(100);
	HAL_UART_Transmit(sens->uart, commands[0], strlen(commands[0]), HAL_MAX_DELAY);
	HAL_Delay(100);


	HAL_UART_Transmit(sens->uart, commands[2], strlen(commands[2]), HAL_MAX_DELAY);
	HAL_Delay(100);


	HAL_UART_Transmit(sens->uart, commands[3], strlen(commands[3]), HAL_MAX_DELAY);
	HAL_Delay(100);

}

void GPSCallbackHandler(){

	gpsSensor.bufor[gpsSensor.buforSize] = gpsSensor.Rx_data;
	gpsSensor.buforSize++;

	  if(gpsSensor.Rx_data == '\n')
	  {
		_dataHandler[GPS].dataReady = 0;
		gpsSensor.bufor[gpsSensor.buforSize]= 0;
		strcpy(gpsSensor.data,gpsSensor.bufor);
		gpsSensor.bufor[0]= '\0';
		gpsSensor.Rx_data = '\0';
		gpsSensor.dataReady = 1;
		_dataHandler[GPS].dataReady = 1;
		gpsSensor.buforSize = 0;
	  }
	  if(gpsSensor.buforSize > 255)
	  {
		 gpsSensor.bufor[0] = '\0';
		 gpsSensor.buforSize = 0;
		 _dataHandler[GPS].dataReady = 0;
	  }

	  HAL_UART_Receive_IT(gpsSensor.uart, &(gpsSensor.Rx_data), 1);
}



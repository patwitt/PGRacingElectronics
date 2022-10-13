/*
 * GPS.h
 *
 *  Created on: 28 wrz 2022
 *      Author: Patryk
 */

#ifndef INC_SENSORS_GPS_H_
#define INC_SENSORS_GPS_H_



#endif /* INC_SENSORS_GPS_H_ */

#include "sdcard/sdmmc.h"
#include "ff.h"
#include "stdio.h"
#include "sensorFunctions.h"
#pragma once

typedef struct GPS{
	FIL *File; //GPS File to write
	char path[20]; // path of file to write;
	uint8_t dataReady : 1; // flag to check if data is ready to read and write to file
	uint8_t saveLock : 1; //Mutex to not copy data when saving to file
	int saveRate;
	uint8_t Rx_data;
	char bufor[255];
	uint8_t buforSize;
	char data[255];
	UART_HandleTypeDef * uart;
}GPSSensor;

void GPSInit(GPSSensor * sens);

void GPSReinitUart(GPSSensor * sens);

void GPSCallbackHandler();

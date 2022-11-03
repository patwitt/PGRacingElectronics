/*
 * GYRO.h
 *
 *  Created on: 28 wrz 2022
 *      Author: Patryk
 */

#ifndef INC_SENSORS_GYRO_H_
#define INC_SENSORS_GYRO_H_



#endif /* INC_SENSORS_GYRO_H_ */
#include "sdcard/sdmmc.h"
#include "ff.h"
#include "stdio.h"
#include "sensorFunctions.h"
#include "sensors/MPU9250.h"

#pragma once
/* *******GYRO SECTION  ********/

typedef struct gyroData
{
	int16_t acc_data[3];
	int16_t mag_data[3];
	int16_t gyro_data[3];
}gyroData;

typedef struct gyroDataCalc
{
	double acc_data_calc[3];
	double gyro_data_calc[3];
}gyroDataCalculated;

typedef struct GYRO{
	FIL *File; //GYRO File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read/write to file
	int saveRate;
	int timeToNextRead;
	gyroDataCalculated data;
	I2C_HandleTypeDef i2c;
}GyroSensor;

//GYRO FUNCS
void gyroInit(GyroSensor * gyro);

void gyroGetData(GyroSensor * sens);

void gyroConvertData(struct gyroData * input, struct gyroDataCalc * output);
/*
 * MLX.h
 *
 *  Created on: 28 wrz 2022
 *      Author: Patryk
 */

#ifndef INC_SENSORS_MLX_H_
#define INC_SENSORS_MLX_H_



#endif /* INC_SENSORS_MLX_H_ */
#include "sdcard/sdmmc.h"
#include "ff.h"
#include "stdio.h"
#include "sensorFunctions.h"
#include "sensors/MLX90640_API.h"
#include "sensors/MLX90640_I2C_Driver.h"
#pragma once
/* *******MLX90640 SECTION  ********/
//MLX DEFINES

#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33 // Default mlx address
#define	 RefreshRate 0x04
#define  TA_SHIFT 8 //Default shift for MLX90640 in open air


//MLX STRUCTS
typedef struct MLX{
	FIL *File; //mlx File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read/write to file
	int saveRate;
	int timeToNextRead;
	int ID ;
	I2C_HandleTypeDef * i2c; // I2C connected to sensor
	paramsMLX90640 mlx90640;
	float data[768]; // data frame array of 768 pixels
	float ambientTemp; // temperature of sensor itself
	float vdd; // Voltage
	float emissivity;

}MLXSensor;


//MLX FUNCS
int mlxInit(MLXSensor *mlx,SENSORS id, I2C_HandleTypeDef * i2c,FIL * f);

//Copy and calcualte data from sensor memory to stm
int mlxGetData(MLXSensor* mlx);

//DEBUG printing on default uart
void mlxPrintData(MLXSensor* mlx);



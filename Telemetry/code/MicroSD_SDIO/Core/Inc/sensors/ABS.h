/*
 * ABS.h
 *
 *  Created on: 28 wrz 2022
 *      Author: Patryk
 */

#ifndef INC_SENSORS_ABS_H_
#define INC_SENSORS_ABS_H_
#endif /* INC_SENSORS_ABS_H_ */

#include "sdcard/sdmmc.h"
#include "ff.h"
#include "stdio.h"
#include "sensorFunctions.h"
#pragma once

/* *******ABS SECTION  ********/


typedef struct ABS{
	FIL *File; //ABS File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read and write to file
	int saveRate;
	float data;
	int ID;
	//ABS data asynchronous
	int timeToZeroSpeed; //after that time if not new input we assume car is not moving
	TIM_HandleTypeDef * timer;
	int timerChannel;
	uint8_t raw[10];
	uint8_t counter;

}ABSSensor;

void absInit(ABSSensor * sens,SENSORS id,TIM_HandleTypeDef * tim,int channel,FIL* f);
void absCalculate(ABSSensor * sens);

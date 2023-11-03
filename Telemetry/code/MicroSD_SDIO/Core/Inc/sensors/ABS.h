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

#pragma once
#define ABS_CONST 0.9454
/* *******ABS SECTION  ********/


typedef struct ABS{
	FIL *File; //ABS File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read and write to file
	int saveRate;
	float data;
	int ID;
	//ABS data asynchronous
	int timeToNextRead; //after that time if not new input we assume car is not moving
	TIM_HandleTypeDef * timer;
	int timerChannel;
	uint8_t rawData[10];
	uint8_t counter;
	int timestamp;

}ABSSensor;

void ABSInit(ABSSensor * sens,int id,TIM_HandleTypeDef * tim,int channel,FIL* f);
void ABSCallbackHandler(TIM_HandleTypeDef *htim);
void ABSCalculate(ABSSensor * sens);
void sendWheelSpeedByCan(int id);

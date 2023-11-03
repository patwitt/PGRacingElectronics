/*
 * ABS.h
 *
 *  Created on: 28 wrz 2022
 *      Author: Patryk
 */

#ifndef INC_SENSORS_ABS_H_
#define INC_SENSORS_ABS_H_
#endif /* INC_SENSORS_ABS_H_ */

#include "ff.h"

#pragma once
#define ABS_NUMBER_OF_POINTS 22.0
#define ABS_TIME_SAMPLES 20
#define ABS_TIME_DIFF 5.0
#define WHEEL_RADIUS 10.16
#define WHEEL_PERIMETER WHEEL_RADIUS * 3.14 * 2.0
#define ABS_SPEED_CM_S (float)((1000.0/(ABS_TIME_DIFF*ABS_TIME_SAMPLES))/ ABS_NUMBER_OF_POINTS)*WHEEL_PERIMETER
#define ABS_CONST ABS_SPEED_CM_S / 100 / 1000.0 *3600
/* *******ABS SECTION  ********/


typedef struct ABS{
	FIL *File; //ABS File to write data
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read and write to file
	int saveRate;
	float data;
	int ID;
	//ABS data asynchronous
	int timeToNextRead;
	TIM_HandleTypeDef * timer;
	int timerChannel;
	uint8_t rawData[ABS_TIME_SAMPLES];
	uint8_t counter;
	int timestamp;

}ABSSensor;

void ABSInit(ABSSensor * sens,int id,TIM_HandleTypeDef * tim,int channel,FIL* f);
void ABSCallbackHandler(TIM_HandleTypeDef *htim);
void ABSCalculate(ABSSensor * sens);
void sendWheelSpeedByCan(int id);

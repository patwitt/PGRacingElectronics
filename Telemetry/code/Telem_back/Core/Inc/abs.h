/*
 * ABS.h
 *
 *  Created on: Aug 11, 2022
 *      Author: Bartosz Janicki
 */

#ifndef __ABS_H__
#define __ABS_H__

#ifdef __cplusplus
extern "C" {
#endif


//#include "main.h"
#include "tim.h"
#include "math.h"

typedef struct ABS{

//	int dataReady; // flag to check if data is ready to read and write to file
//	int saveRate;
	volatile uint32_t data;
	int ID;
	int timeToZeroSpeed; //after that time if not new input we assume car is not moving
	TIM_HandleTypeDef * timer;
	int timerChannel;
	uint8_t raw[10];
	uint8_t counter;
}ABSSensor;

extern ABSSensor absLRSensor;
extern ABSSensor absRRSensor;
extern uint8_t flagL;
extern uint8_t flagR;
/* INIT FUNCTION */
void absInit(ABSSensor * sens,int id ,TIM_HandleTypeDef* htim,int channel);

/* CALLBACK FUNCTION */
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim);

/* VALUE CALC */
void absCalculate( ABSSensor * sens);

#ifdef __cplusplus
}
#endif


#endif /* INC_ABS_H_ */

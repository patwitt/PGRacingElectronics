/*
 * Break_pressure.h
 *
 *  Created on: Aug 11, 2022
 *      Author: Bartosz Janicki
 */

#ifndef __BREAK_PRESSURE_H__
#define __BREAK_PRESSURE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define V_THRESHOLD 0.45
#define V_adc 3.3/4096

//#include "main.h"
#include "adc.h"



typedef struct BP{
	int data;
	int ID;
	ADC_HandleTypeDef * adc;
	int adc_channel;
}BPSensor;

extern BPSensor BP_Front_Sensor;
extern BPSensor BP_Rear_Sensor;

void BPInit(BPSensor * sens,int id,ADC_HandleTypeDef* adc_h,int channel);


/* CHANNEL SELECT */
void ADC_Select_CH8 (ADC_HandleTypeDef* adc);

/* CHANNEL SELECT */
void ADC_Select_CH9 (ADC_HandleTypeDef* adc);



/* VALUE CALCULATE */
float BPCalculate(int data);

/* READ DATA  */
//uint16_t BP_GetData(ADC_HandleTypeDef* adc , uint16_t val[2]);
void BP_GetData(BPSensor * sens);


#ifdef __cplusplus
}
#endif

#endif /* INC_BREAK_PRESSURE_H_ */

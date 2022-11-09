/*
 * Amorki.h
 *
 *  Created on: Aug 14, 2022
 *      Author: Bartosz Janicki
 */

#ifndef __AMORKI_H__
#define __AMORKI_H__

#ifdef __cplusplus
extern "C" {
#endif

#define V_adc 3.3/4096

//#include "main.h"
#include "adc.h"

typedef struct Damp{
	int data;
	int ID;
	ADC_HandleTypeDef * adc;
	int adc_channel;
}DamperSensor;

extern DamperSensor DampLRSensor;
extern DamperSensor DampRRSensor;


/* INIT */
void DampInit(DamperSensor * sens,int id,ADC_HandleTypeDef* adc_h,int channel);

/* CHANNEL SELECT */
void ADC_Select_CH5 (ADC_HandleTypeDef* adc);

/* CHANNEL SELECT */
void ADC_Select_CH6 (ADC_HandleTypeDef* adc);

/*  VALUE CALC */
float AmorkiCalculate(int data);
/* VALUE READ  */
//uint16_t Amorki_GetData(ADC_HandleTypeDef* adc , uint16_t val[2]);
void Amorki_GetData(DamperSensor * sens);


#ifdef __cplusplus
}
#endif


#endif /* INC_AMORKI_H_ */

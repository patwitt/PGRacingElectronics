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


//#include "main.h"
#include "adc.h"

/* CHANNEL SELECT */
void ADC_Select_CH5 (ADC_HandleTypeDef* adc);

/* CHANNEL SELECT */
void ADC_Select_CH6 (ADC_HandleTypeDef* adc);

/*  VALUE CALC */
float AmorkiCalculate(int data);
/* VALUE READ  */
uint16_t Amorki_GetData(ADC_HandleTypeDef* adc , uint16_t val[2]);

#ifdef __cplusplus
}
#endif


#endif /* INC_AMORKI_H_ */

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


//#include "main.h"
#include "adc.h"

/* CHANNEL SELECT */
void ADC_Select_CH8 (ADC_HandleTypeDef* adc);

/* CHANNEL SELECT */
void ADC_Select_CH9 (ADC_HandleTypeDef* adc);

/* VALUE CALCULATE */
float BPCalculate(int data);

/* READ DATA  */
uint16_t BP_GetData(ADC_HandleTypeDef* adc , uint16_t val[2]);

#ifdef __cplusplus
}
#endif

#endif /* INC_BREAK_PRESSURE_H_ */

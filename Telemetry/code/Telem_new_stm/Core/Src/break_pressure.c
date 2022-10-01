//#include "adc.h"
#include <amorki.h>




void ADC_Select_CH8 (ADC_HandleTypeDef* adc)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_8;
	  if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH9 (ADC_HandleTypeDef* adc)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_9;
	  if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

uint16_t BP_GetData(ADC_HandleTypeDef* adc , uint16_t val[2])
{
	ADC_Select_CH8(adc);
	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, 1000);
	val[0] = HAL_ADC_GetValue(adc);
	HAL_ADC_Stop(adc);
	ADC_Select_CH9(adc);
	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, 1000);
	val[1] = HAL_ADC_GetValue(adc);
	HAL_ADC_Stop(adc);

	return val;
}




float BPCalculate(int data)
{
	float BP_voltage=data*3.3/4096;		 //ADC TO VOLTAGE
	float res=(BP_voltage-0.45)/0.0325;  //VOLTAGE TO PRESSURE
	return res;
}

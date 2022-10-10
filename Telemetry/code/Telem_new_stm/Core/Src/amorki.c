
//#include "adc.h"
#include <amorki.h>




void ADC_Select_CH5 (ADC_HandleTypeDef* adc)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_5;
	  if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH6 (ADC_HandleTypeDef* adc)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_6;
	  if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

uint16_t Amorki_GetData(ADC_HandleTypeDef* adc , uint16_t val[2])
{
	ADC_Select_CH5(adc);
	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, 1000);
	val[0] = HAL_ADC_GetValue(adc);
	HAL_ADC_Stop(adc);
	ADC_Select_CH6(adc);
	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, 1000);
	val[1] = HAL_ADC_GetValue(adc);
	HAL_ADC_Stop(adc);

	return val;
}




float AmorkiCalculate(int data)
{
	float Amr_voltage=data*3.3/4096;	 //ADC TO VOLTAGE
	float res=(Amr_voltage)*30.3; 		 //VOLTAGE TO DISPLACEMENT
	return res;
}



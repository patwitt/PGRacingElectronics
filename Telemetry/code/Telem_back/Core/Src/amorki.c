
//#include "adc.h"
#include <amorki.h>


//uint32_t adc_data[2];

DamperSensor DampLRSensor;
DamperSensor DampRRSensor;





void ADC_Select_CH5 (ADC_HandleTypeDef* adc)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_5;
	  sConfig.Rank = 1;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH6 (ADC_HandleTypeDef* adc)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_6;
	  sConfig.Rank = 1;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
/*
uint16_t Amorki_GetData(ADC_HandleTypeDef* adc , uint16_t val[2])
{
	ADC_Select_CH5(adc);
	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, HAL_MAX_DELAY);
	val[0] = HAL_ADC_GetValue(adc);
	//adc_data[0]=HAL_ADC_GetValue(adc);
	HAL_ADC_Stop(adc);

	ADC_Select_CH6(adc);
	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, HAL_MAX_DELAY);
	val[1] = HAL_ADC_GetValue(adc);
	//adc_data[1]=HAL_ADC_GetValue(adc);
	HAL_ADC_Stop(adc);

	return val;
}
*/

/* USING STRUCT */

void DampInit(DamperSensor * sens,int id,ADC_HandleTypeDef* adc_h,int channel){
	sens->ID = id;
	sens->adc = adc_h;
	sens->adc_channel = channel;
}

void ADC_SelectChannel(DamperSensor* sens)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = sens->adc_channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(sens->adc, &sConfig) != HAL_OK)
  {
   Error_Handler();
  }
}


void Amorki_GetData(DamperSensor * sens)
{
	switch (sens->adc_channel)
	{
	case ADC_CHANNEL_5:
		ADC_SelectChannel(sens);
		HAL_ADC_Start(sens->adc);
		HAL_ADC_PollForConversion(sens->adc, HAL_MAX_DELAY);
		sens->data = HAL_ADC_GetValue(sens->adc);
		HAL_ADC_Stop(sens->adc);
	break;
	case ADC_CHANNEL_6:
		ADC_SelectChannel(sens);
		HAL_ADC_Start(sens->adc);
		HAL_ADC_PollForConversion(sens->adc, HAL_MAX_DELAY);
		sens->data = HAL_ADC_GetValue(sens->adc);
		HAL_ADC_Stop(sens->adc);
	break;
	}

}



float AmorkiCalculate(int data)
{
	float Amr_voltage=data*V_adc;	 //ADC TO VOLTAGE
	float res=(Amr_voltage)*30.3; 		 //VOLTAGE TO DISPLACEMENT
	return res;
}



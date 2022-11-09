//#include "adc.h"
#include <break_pressure.h>


BPSensor BP_Front_Sensor;
BPSensor BP_Rear_Sensor;



void ADC_Select_CH8 (ADC_HandleTypeDef* adc)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_8;
	  sConfig.Rank = 1;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH9 (ADC_HandleTypeDef* adc)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_9;
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
uint16_t BP_GetData(ADC_HandleTypeDef* adc , uint16_t val[2])
{
	ADC_Select_CH8(adc);
	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, HAL_MAX_DELAY);
	val[0] = HAL_ADC_GetValue(adc);
	HAL_ADC_Stop(adc);
	ADC_Select_CH9(adc);
	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, HAL_MAX_DELAY);
	val[1] = HAL_ADC_GetValue(adc);
	HAL_ADC_Stop(adc);

	return val;
}

/* USING STRUCT */

void BPInit(BPSensor * sens,int id,ADC_HandleTypeDef* adc_h,int channel){
	sens->ID = id;
	sens->adc = adc_h;
	sens->adc_channel = channel;
	sens->data=0;
}

void ADC_BSelectChannel(BPSensor* sens)
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


void BP_GetData(BPSensor * sens)
{
	switch (sens->adc_channel)
	{
	case ADC_CHANNEL_8:
		ADC_BSelectChannel(sens);
		HAL_ADC_Start(sens->adc);
		HAL_ADC_PollForConversion(sens->adc, HAL_MAX_DELAY);
		sens->data = HAL_ADC_GetValue(sens->adc);
		HAL_ADC_Stop(sens->adc);
	break;
	case ADC_CHANNEL_9:
		ADC_BSelectChannel(sens);
		HAL_ADC_Start(sens->adc);
		HAL_ADC_PollForConversion(sens->adc, HAL_MAX_DELAY);
		sens->data = HAL_ADC_GetValue(sens->adc);
		HAL_ADC_Stop(sens->adc);
	break;
	}

}


float BPCalculate(int data)
{
	float BP_voltage=data*V_adc;		 //ADC TO VOLTAGE
	float res=(BP_voltage-V_THRESHOLD)/0.0325;  //VOLTAGE TO PRESSURE
	return res;
}

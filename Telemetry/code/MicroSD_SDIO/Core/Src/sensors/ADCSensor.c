/*
 * ADCSensor.c
 *
 *  Created on: 28 wrz 2022
 *      Author: Patryk
 */
#include "sensors/ADCSensor.h"
#include "handler.h"
/* *******ADC SECTION  ********/

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern RTC_HandleTypeDef hrtc;
extern sensorDataHandler _dataHandler[];
extern SensorStatus statusRegister;

void adcInit(ADCSensor* sens,ADC_HandleTypeDef * adc,int channel,FIL* f)
{
	if(f == 0)
	{
		sens->File = (FIL*)malloc(sizeof(FIL));
	}else
	{
		sens->File = f;
	}
	sens->adcChannel = channel;
	sens->adc = adc;
	adcSetActiveChannel(sens);
	HAL_ADC_Start_DMA(adc, &sens->data, 1);
}
void damperInit(ADCSensor* sens,int id,FIL * f){
	switch(id){
	case DAMPERLF:
		adcInit(sens,&hadc1,ADC_CHANNEL_0,f);
		statusRegister.DamperLF = SENSOR_OK;
		break;
	case DAMPERRF:
		adcInit(sens,&hadc2,ADC_CHANNEL_1,f);
		statusRegister.DamperRF = SENSOR_OK;
		break;
	default:
		break;
	}
	sens->timeToNextRead = 50;
	sens->dataReady = 0;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"DAMP%02d%02d.csv",date.Date,date.Month);
	sens->ID = id;
	sens->adcType = damper;

}
void steeringInit(ADCSensor* sens){

	adcInit(sens,&hadc1,0,0);
	statusRegister.Steering = SENSOR_OK;
	sens->ID = WHEEL;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"WHEEl%02d%02d.csv",date.Date,date.Month);
	sens->adcType = steeringWheel;
}
void adcSetActiveChannel(ADCSensor* sens)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = sens->adcChannel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(sens->adc, &sConfig) != HAL_OK)
  {
   Error_Handler();
  }
}
void adcGetData(ADCSensor * sens){
	adcSetActiveChannel(sens);
	HAL_ADC_Start(sens->adc);
	HAL_ADC_PollForConversion(sens->adc, HAL_MAX_DELAY);
	sens->data = HAL_ADC_GetValue(sens->adc);
	_dataHandler[sens->ID].dataReady = 1;
}
void adcSendData(ADCSensor * sens)
{
	printf("[%d] ADC: %d", HAL_GetTick(), sens->data);
}

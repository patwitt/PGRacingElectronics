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
uint16_t adc1[2];
uint16_t adc2[2];
uint16_t adc3[3];
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
	//ADC_SetActiveChannel(sens);
	if(sens->ID == DAMPERLF)
		HAL_ADC_Start_DMA(adc, adc1, 2);
	if(sens->ID == DAMPERRF )
		HAL_ADC_Start_DMA(adc, adc2, 2);
	if(sens->ID == WHEEL)
		HAL_ADC_Start_DMA(adc, adc3, 2);


}
void damperInit(ADCSensor* sens,int id,FIL * f){
	sens->ID = id;
	switch(id){
	case DAMPERLF:
		adcInit(sens,&hadc1,ADC_CHANNEL_2,f);
		statusRegister.DamperLF = SENSOR_OK;
		sens->data = adc1;
		break;
	case DAMPERRF:
		adcInit(sens,&hadc2,ADC_CHANNEL_0,f);
		statusRegister.DamperRF = SENSOR_OK;
		sens->data = adc2;
		break;
	case DAMPERLR:
		adcInit(sens,&hadc1,ADC_CHANNEL_3,f);
		sens->data = adc1+1;
		//statusRegister.DamperLF = SENSOR_OK;
		break;
	case DAMPERRR:
		adcInit(sens,&hadc2,ADC_CHANNEL_1,f);
		sens->data = adc2+1;
		//statusRegister.DamperRF = SENSOR_OK;
		break;
	default:
		break;
	}
	sens->timeToNextRead = 50;
	sens->dataReady = 0;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"DAMP%02d%02d.csv",date.Date,date.Month);

	sens->adcType = damper;

}
void steeringInit(ADCSensor* sens){

	adcInit(sens,&hadc3,5,0);
	statusRegister.Steering = SENSOR_OK;
	sens->ID = WHEEL;
	sens->data = adc3+1;
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
void adcSendDataByCan(ADCSensor * sens){
	uint32_t* TxMailBox = 0;
	CAN_TxHeaderTypeDef pHeader;
	pHeader.DLC = 2;
	pHeader.IDE = CAN_ID_STD;
	pHeader.StdId = 0x560 + sens->ID;
	pHeader.RTR = CAN_RTR_DATA;
	uint16_t data = *(sens->data);

	HAL_CAN_AddTxMessage(&hcan1, &pHeader,&data , TxMailBox);
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

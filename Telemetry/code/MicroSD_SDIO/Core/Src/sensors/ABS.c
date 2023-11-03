/*
 * ABS.c
 *
 *  Created on: 28 wrz 2022
 *      Author: Patryk
 */
#include "sensors/ABS.h"
#include "math.h"
#include "handler.h"

extern RTC_HandleTypeDef hrtc;
extern SensorStatus statusRegister;
extern sensorDataHandler _dataHandler[];
extern ABSSensor absLFSensor;
extern ABSSensor absRFSensor;
/******** ABS SECTION  ********/
void ABSInit(ABSSensor * sens,int id,TIM_HandleTypeDef* tim,int channel,FIL *f){
	if(f == 0)
	{
		sens->File = (FIL*)malloc(sizeof(FIL));
	}else
	{
		sens->File = f;
	}
	switch(id){
	case ABSLF:
		statusRegister.VSSLF = SENSOR_OK;
		break;

	case ABSRF:
		statusRegister.VSSRF = SENSOR_OK;
		break;
	}
	sens->dataReady = 0;
	sens->ID =id;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"ABS%02d%02d.csv",date.Date,date.Month);
	sens->timer = tim;
	sens->timerChannel = channel;
	for(int i=0;i<10;i++){
		sens->rawData[i] = 0;
	}
	sens->counter = 0;
	sens->data = 0;
}

void ABSCalculate(ABSSensor * sens)
{
	sens->data = 0;
	 for(int i=0;i<10;i++)
	 {
		 sens->data += sens->rawData[i];
	 }
	 sens->data *= ABS_CONST;
	 sens->counter++;
	 if(sens->counter>=10){
		 sens->counter = 0;
	 }
	 sens->rawData[sens->counter] = 0;


	 sendWheelSpeedByCan(sens->ID);
}
void ABSCallbackHandler(TIM_HandleTypeDef *htim){
	if (htim == absLFSensor.timer) {
	    switch (HAL_TIM_GetActiveChannel(absLFSensor.timer)) {
	      case HAL_TIM_ACTIVE_CHANNEL_1:
	    	  absLFSensor.rawData[absLFSensor.counter]++;//HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	    	 // _dataHandler[ABSLF].dataReady = 1;
	        break;
	      default:
	        break;
	    }
	  }else if(htim == absRFSensor.timer) {
	      switch (HAL_TIM_GetActiveChannel(absRFSensor.timer)) {
	        case HAL_TIM_ACTIVE_CHANNEL_1:
	        	absRFSensor.rawData[absRFSensor.counter]++;//HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	        	//_dataHandler[ABSRF].dataReady = 1;
	          break;
	        default:
	          break;
	       }
	  }
}

void sendWheelSpeedByCan(int id){
	ABSSensor* sensor = (ABSSensor*)_dataHandler[id].sensorStruct;
	uint32_t* TxMailBox = 0;
	CAN_TxHeaderTypeDef pHeader;
	pHeader.DLC = 2;
	pHeader.IDE = CAN_ID_STD;
	pHeader.StdId = 0x560 + id;
	pHeader.RTR = CAN_RTR_DATA;
	uint16_t data =(uint16_t)sensor->data;
	HAL_StatusTypeDef res =HAL_CAN_AddTxMessage(&hcan2, &pHeader,&data , TxMailBox);
}

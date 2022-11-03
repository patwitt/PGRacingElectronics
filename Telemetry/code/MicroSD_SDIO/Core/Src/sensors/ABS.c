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
ABSSensor absLFSensor;
ABSSensor absRFSensor;
/******** ABS SECTION  ********/
void absInit(ABSSensor * sens,SENSORS id,TIM_HandleTypeDef* tim,int channel,FIL *f){
	if(f == 0)
	{
		sens->File = (FIL*)malloc(sizeof(FIL));
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
}

float absCalculate(int time)
{
//Prędkośc = czas * (średnica*kąt*PI)
	float res = (1.0/22)*1.43*(float)(time)*M_PI;
	return res;
}
void ABSCallbackHandler(TIM_HandleTypeDef *htim){
	if (htim == absLFSensor.timer) {
	    switch (HAL_TIM_GetActiveChannel(absLFSensor.timer)) {
	      case HAL_TIM_ACTIVE_CHANNEL_1:
	    	  absLFSensor.data++;//HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	    	  absLFSensor.timeToZeroSpeed = ABS_ZERO_SPEED_TIME;
	    	  _dataHandler[ABSLF].dataReady = 1;
	    	  absLFSensor.dataReady = 1;
	        break;
	      default:
	        break;
	    }
	  }else if(htim == absRFSensor.timer) {
	      switch (HAL_TIM_GetActiveChannel(absRFSensor.timer)) {
	        case HAL_TIM_ACTIVE_CHANNEL_1:
	        	absRFSensor.data++;//HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	        	absRFSensor.timeToZeroSpeed = ABS_ZERO_SPEED_TIME;
	        	_dataHandler[ABSRF].dataReady = 1;
	        	absRFSensor.dataReady = 1;
	          break;
	        default:
	          break;
	       }
	  }
}

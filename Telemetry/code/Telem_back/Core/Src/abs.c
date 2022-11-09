//#include "math.h"
//#include "tim.h"
#include <abs.h>

typedef enum SensorsDeagultTimings{
	//MLX_DATA_RATE = 1000,
	//DAMPER_DATA_RATE = 50,


	ABS_ZERO_SPEED_TIME = 100

};

ABSSensor absRRSensor;
ABSSensor absLRSensor;
uint8_t flagL=0x0;
uint8_t flagR=0x0;

void absInit(ABSSensor * sens,int id,TIM_HandleTypeDef* tim,int channel){
	sens->ID = id;
	sens->timer = tim;
	sens->timerChannel = channel;
	//HAL_TIM_Base_Start_IT(tim);
	HAL_TIM_IC_Start_IT(tim, channel);
	for(int i=0;i<10;i++){
			sens->raw[i] = 0;
		}
		sens->counter = 0;
		sens->data = 0;
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim == absLRSensor.timer) {

    switch (HAL_TIM_GetActiveChannel(absLRSensor.timer)) {
      case HAL_TIM_ACTIVE_CHANNEL_1:
    	 // absLRSensor.data = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    	 //absLRSensor.timeToZeroSpeed = ABS_ZERO_SPEED_TIME;
    	 //flagL=0x01;
    	  absLRSensor.raw[absLRSensor.counter]++;

        break;
      default:
        break;
    }
  }else if(htim == absRRSensor.timer) {

	  switch (HAL_TIM_GetActiveChannel(absRRSensor.timer)) {
	  	  case HAL_TIM_ACTIVE_CHANNEL_1:
        	//absRRSensor.data = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        	//absRRSensor.timeToZeroSpeed = ABS_ZERO_SPEED_TIME;
        	//flagR=0x01;
	  		  absRRSensor.raw[absRRSensor.counter]++;

            break;
          default:
            break;
        }
  }

}


void absCalculate(ABSSensor * sens)
{
/*
//angle 1/22
//diamter 457.2mm
//Prędkośc[km/h] = (angle*diameter[m]*PI)/time[x0us]->[s]
	float numerator = 0.04545*0.4572*M_PI;
	float time=data;
	float res = 3.6 * numerator/(time/100000);
	//flag=0x00;
	return res;
	*/
	sens->data = 0;
	 for(int i=0;i<10;i++)
	 {
		 sens->data += sens->raw[i];
	 }
	 sens->data *= 0.9454;
	 sens->counter++;
	 if(sens->counter>=10){
		 sens->counter = 0;
	 }
	 sens->raw[sens->counter] = 0;

}


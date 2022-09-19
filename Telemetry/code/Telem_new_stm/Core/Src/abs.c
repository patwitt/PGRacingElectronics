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
uint8_t flag=0x0;

void absInit(ABSSensor * sens,int id,TIM_HandleTypeDef* tim,int channel){
	sens->ID = id;
	sens->timer = tim;
	sens->timerChannel = channel;
	HAL_TIM_Base_Start(tim);
	HAL_TIM_IC_Start_IT(tim, channel);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim == absLRSensor.timer) {
    switch (HAL_TIM_GetActiveChannel(absLRSensor.timer)) {
      case HAL_TIM_ACTIVE_CHANNEL_1:
    	  absLRSensor.data = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    	  absLRSensor.timeToZeroSpeed = ABS_ZERO_SPEED_TIME;
    	  flag=0x01;

        break;
      default:
        break;
    }
  }else if(htim == absRRSensor.timer) {
	  switch (HAL_TIM_GetActiveChannel(absRRSensor.timer)) {
	  	  case HAL_TIM_ACTIVE_CHANNEL_1:
        	  absRRSensor.data = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        	  absRRSensor.timeToZeroSpeed = ABS_ZERO_SPEED_TIME;
        	  flag=0x02;

            break;
          default:
            break;
        }
  }

}



float absCalculate(int data)
{
//angle 1/22
//diamter 457.2mm
//Prędkośc = kat * (średnica*czas*PI)

	float res = (1/22)*45.7*data*M_PI;
	flag=0x00;
	return res;
}


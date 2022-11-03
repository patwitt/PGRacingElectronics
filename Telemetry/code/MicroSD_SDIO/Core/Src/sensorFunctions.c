/*
 * sensorFunctions.h
 *
 *  Created on: Jun 17, 2022
 *      Author: Patryk
 */

//INCLUDES

#include "sensorFunctions.h"
#include "sensors/ABS.h"
#include "sensors/ADCSensor.h"
#include "sensors/GPS.h"
#include "sensors/GYRO.h"
#include "sensors/MLX.h"
#include "tim.h"
#include "can.h"
#include "ecumaster.h"
#include "handler.h"
extern UART_HandleTypeDef huart7;

/* *******GLOBAL VARIABLES ********/
extern sensorDataHandler _dataHandler[];
extern ABSSensor absLFSensor;
extern ABSSensor absRFSensor;
extern GPSSensor gpsSensor;
extern EcumasterData EcuData;
MLXSensor mlxLFSensor;
MLXSensor mlxRFSensor;
GyroSensor gyro;
SensorStatus statusRegister;
ADCSensor sWheelSensor;
ADCSensor damperLFSensor;
ADCSensor damperRFSensor;

void enumToSensor(char *buf,SENSORS sensor){
	switch(sensor){
	case GPS:
		strcpy(buf,"GPS");
	case GYRO:
		strcpy(buf,"GYRO");
	case MLXLF:
		strcpy(buf,"MLXLF");
	case MLXRF:
		strcpy(buf,"MLXRF");
	case ABSLF:
		strcpy(buf,"ABSLF");
	case ABSRF:
		strcpy(buf,"ABSRF");
	case WHEEL:
		strcpy(buf,"STEER");
	case DAMPERLF:
		strcpy(buf,"DAMPLF");
	case DAMPERRF:
		strcpy(buf,"DAMPRF");
	case ECU:
		strcpy(buf,"ECU");

	}
}

void initSensors()
{


	if(_dataHandler[GYRO].isActive){
		gyroInit(&gyro);
	}
	if(_dataHandler[GPS].isActive){
		 GPSInit(&gpsSensor);
	}
	if(_dataHandler[MLXLF].isActive){
		mlxInit(&mlxLFSensor,MLXLF,&hi2c1,0);
	}
	if(_dataHandler[MLXRF].isActive){
		mlxInit(&mlxRFSensor,MLXRF,&hi2c3,mlxRFSensor.File);
	}
	if(_dataHandler[ABSLF].isActive){
	    HAL_TIM_Base_Start(&htim3);
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
		absInit(&absLFSensor, ABSLF, &htim3, TIM_CHANNEL_1, 0);
	}
	if(_dataHandler[ABSRF].isActive){
	    HAL_TIM_Base_Start(&htim4);
		HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
		absInit(&absRFSensor, ABSRF, &htim4, TIM_CHANNEL_1, absLFSensor.File);
	}
	if(_dataHandler[WHEEL].isActive){
		steeringInit(&sWheelSensor);
	}
	if(_dataHandler[DAMPERLF].isActive){
		damperInit(&damperLFSensor, DAMPERLF, 0);
	}
	if(_dataHandler[DAMPERRF].isActive){
		damperInit(&damperRFSensor, DAMPERRF, damperLFSensor.File);
	}
	if(_dataHandler[ECU].isActive){
		  EcuInit(&EcuData);
	}
	HAL_CAN_Start(&hcan2);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

}
int statusToInt()
{	int fullRegister = statusRegister.TeleBack;
	fullRegister = fullRegister << 2;
	fullRegister =  (fullRegister  |statusRegister.DamperRF) << 3;
	fullRegister =  (fullRegister  |statusRegister.DamperLF) << 3;
	fullRegister =  (fullRegister  |statusRegister.Steering) << 3;
	fullRegister =  (fullRegister  |statusRegister.VSSRF) << 3;
	fullRegister =  (fullRegister  |statusRegister.VSSLF) << 3;
	fullRegister =  (fullRegister  |statusRegister.MLXRF) << 3;
	fullRegister =  (fullRegister  |statusRegister.MLXLF) << 3;
	fullRegister = (fullRegister  | statusRegister.GYRO) << 3;
	fullRegister = (fullRegister  | statusRegister.GPS) << 3;
	fullRegister =  (fullRegister  |statusRegister.SDCARD);
	return fullRegister;
}
void printStatusRegister()
{
	int fullRegister = statusToInt();
	int b =0;
	for(int i=31;i>=0;i--)
	{
		b = fullRegister >> i;
		if(b & 1)
		{
			printf("1");
		}else
		{
			printf("0");
		}
		if(i%3 == 0)
		{
			printf(" ");
		}

	}
	printf("\n");
};

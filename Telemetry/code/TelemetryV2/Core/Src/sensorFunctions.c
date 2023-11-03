/*
 * sensorFunctions.h
 *
 *  Created on: Jun 17, 2022
 *      Author: Patryk
 */

//INCLUDES
#include "basicFunctions.h"
#include "sensorFunctions.h"
#include "handler.h"

extern UART_HandleTypeDef huart7;

/* *******GLOBAL VARIABLES ********/
extern sensorDataHandler _dataHandler[];
ABSSensor absLFSensor;
ABSSensor absRFSensor;
GPSSensor gpsSensor;

MLXSensor mlxLFSensor;
MLXSensor mlxRFSensor;
IMUSensor IMUInnerSensor;
SensorStatus statusRegister;
ADCSensor sWheelSensor;
ADCSensor damperLFSensor;
ADCSensor damperRFSensor;
ADCSensor damperLRSensor;
ADCSensor damperRRSensor;
TeleBackData teleData;
EcumasterData ecuData;


const char* enumToSensor(SENSORS sensor){
	switch(sensor){
	case GPS:
		return "GPS";
	case IMU:
		return "IMU";
	case MLXLF:
		return "MLXLF";
	case MLXRF:
		return "MLXRF";
	case ABSLF:
		return "ABSLF";
	case ABSRF:
		return "ABSRF";
	case WHEEL:
		return "STEER";
	case DAMPERLF:
		return"DAMPLF";
	case DAMPERRF:
		return "DAMPRF";
	case ECU:
		return "ECU";
	case TELEBACK:
		return "BACK";
	default:
		return "";

	}
}

void initSensors()
{

	if(_dataHandler[IMU].isActive){
		IMUInit(&IMUInnerSensor);
	}
	if(_dataHandler[GPS].isActive){
		 GPSInit(&gpsSensor);
	}
	if(_dataHandler[MLXLF].isActive){
		mlxInit(&mlxLFSensor,MLXLF,&hi2c2,0);
	}
	if(_dataHandler[MLXRF].isActive){
		mlxInit(&mlxRFSensor,MLXRF,&hi2c4,mlxLFSensor.File);
	}
	if(_dataHandler[ABSLF].isActive){
	    HAL_TIM_Base_Start(&htim3);
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
		ABSInit(&absLFSensor, ABSLF, &htim3, TIM_CHANNEL_4, 0);
	}
	if(_dataHandler[ABSRF].isActive){
	    HAL_TIM_Base_Start(&htim2);
	    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
		ABSInit(&absRFSensor, ABSRF, &htim2, TIM_CHANNEL_1, absLFSensor.File);
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
	if(_dataHandler[DAMPERLR].isActive){
		damperInit(&damperLRSensor, DAMPERLR, damperLFSensor.File);
	}
	if(_dataHandler[DAMPERRR].isActive){
		damperInit(&damperRRSensor, DAMPERRR, damperLFSensor.File);
	}
	if(_dataHandler[ECU].isActive){
		  EcuInit(&ecuData);
	}
	if(_dataHandler[TELEBACK].isActive){
		TeleBackInit(&teleData);
	}
	//Check if can still works!

    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);


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
	fullRegister = (fullRegister  | statusRegister.IMU) << 3;
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

/*
 * handler.c
 *
 *  Created on: 12 sie 2022
 *      Author: Patryk
 */
#include "handler.h"


sensorDataHandler _dataHandler[SENSORS_N] = {
		{
				.sensorType = GPS,
				.sensorStruct = (void*)(&gpsSensor),
				.getDataHandler = NULL,
				.saveDataHandler = gpsSaveData,
				.dataReady = 0,
				.isActive = 1,
		},
		{
				.sensorType = GYRO,
				.sensorStruct = (void*)(&gyro),
				.getDataHandler = gyroGetData,
				.saveDataHandler = gyroSaveData,
				.dataReady = 0,
				.isActive = 1,
		},
		{
				.sensorType = MLXLF,
				.sensorStruct = (void*)(&mlxLFSensor),
				.getDataHandler = mlxGetData,
				.saveDataHandler = mlxSaveData,
				.dataReady = 0,
				.isActive = 0,
		},
		{
				.sensorType = MLXRF,
				.sensorStruct = (void*)(&mlxLFSensor),
				.getDataHandler = mlxGetData,
				.saveDataHandler = mlxSaveData,
				.dataReady = 0,
				.isActive = 0,
		},
		{
				.sensorType = WHEEL,
				.sensorStruct = (void*)(&sWheelSensor),
				.getDataHandler = mlxGetData,
				.saveDataHandler = mlxSaveData,
				.dataReady = 0,
				.isActive = 0,
		},
		{
				.sensorType = DAMPERLF,
				.sensorStruct = (void*)(NULL),
				.getDataHandler = NULL,
				.saveDataHandler = NULL,
				.dataReady = 0,
				.isActive = 0,
		},
		{
				.sensorType = DAMPERRF,
				.sensorStruct = (void*)(NULL),
				.getDataHandler = NULL,
				.saveDataHandler = NULL,
				.dataReady = 0,
				.isActive = 0,
		},


};

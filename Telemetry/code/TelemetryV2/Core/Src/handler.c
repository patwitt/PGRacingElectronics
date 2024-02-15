/*
 * handler.c
 *
 *  Created on: 12 sie 2022
 *      Author: Patryk
 */
#include "handler.h"
#include "sdcard/SDCARD.h"


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
				.sensorType = IMU,
				.sensorStruct = (&IMUInnerSensor),
				.getDataHandler = IMUGetData,
				.saveDataHandler = IMUSaveData,
				.dataReady = 0,
				.isActive = 0,
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
				.sensorStruct = (void*)(&mlxRFSensor),
				.getDataHandler = mlxGetData,
				.saveDataHandler = mlxSaveData,
				.dataReady = 0,
				.isActive = 0,
		},
		{
				.sensorType = ABSLF,
				.sensorStruct = (void*)(&absLFSensor),
				.getDataHandler = NULL,
				.saveDataHandler = absSaveData,
				.dataReady = 0,
				.isActive = 1,
		},
		{
				.sensorType = ABSRF,
				.sensorStruct = (void*)(&absRFSensor),
				.getDataHandler = NULL,
				.saveDataHandler = absSaveData,
				.dataReady = 0,
				.isActive = 1,
		},
		{
				.sensorType = WHEEL,
				.sensorStruct = (void*)(&sWheelSensor),
				.getDataHandler = 0,//adcGetData,
				.saveDataHandler = adcSaveData,
				.dataReady = 0,
				.isActive = 0,
		},
		{
				.sensorType = DAMPERLF,
				.sensorStruct = (void*)(&damperLFSensor),
				.getDataHandler = 0,//adcGetData,
				.saveDataHandler = adcSaveData,
				.dataReady = 0,
				.isActive = 1,
		},
		{
				.sensorType = DAMPERRF,
				.sensorStruct = (void*)(&damperRFSensor),
				.getDataHandler = 0,//adcGetData,
				.saveDataHandler = adcSaveData,
				.dataReady = 0,
				.isActive = 1,
		},
		{
				.sensorType = ECU,
				.sensorStruct = (void*)(&ecuData),
				.getDataHandler = NULL,
				.saveDataHandler = ecuSaveData,
				.dataReady = 0,
				.isActive = 1,
		},
		{
				.sensorType = TELEBACK,
				.sensorStruct = (void*)(&teleData),
				.getDataHandler = NULL,
				.saveDataHandler = telebackSaveData,
				.dataReady = 0,
				.isActive = 0,
		},
		{
				.sensorType = DAMPERLR,
				.sensorStruct = (void*)(&damperLRSensor),
				.getDataHandler = 0,//adcGetData,
				.saveDataHandler = adcSaveData,
				.dataReady = 0,
				.isActive = 1,
		},
		{
				.sensorType = DAMPERRR,
				.sensorStruct = (void*)(&damperRRSensor),
				.getDataHandler = 0,//adcGetData,
				.saveDataHandler = adcSaveData,
				.dataReady = 0,
				.isActive = 1,
		},


};

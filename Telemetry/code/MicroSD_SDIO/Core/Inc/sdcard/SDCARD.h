/*
 * SDCARD.h
 *
 *  Created on: 27 cze 2022
 *      Author: Patryk
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include "sensorFunctions.h"
#include "sensors/ABS.h"
#include "sensors/ADCSensor.h"
#include "sensors/GPS.h"
#include "sensors/GYRO.h"
#include "sensors/MLX.h"
#include "ecumaster.h"

#pragma once
typedef enum{
	SD_OK = 1,
	SD_WRITE_ERROR = -1,
	SD_READ_ERROR = -2,
	WRONG_PARAMETER = -3,
	SD_OPEN_ERROR = -4,
}SD_STATUS;

void sdInit(FATFS* fs);

void openAllFiles();

int createHeaders(FIL * file,char * path);

int openFile(FIL * file, char * path, BYTE mode);

void statsSave(int operation, int time, int sensor);

void telebackSaveData(TeleBackData * teleBack);

void ecuSaveData(EcumasterData ecu);

void IMUSaveData(IMUSensor* sens);

void mlxSaveData(MLXSensor* mlx);

void absSaveData(ABSSensor* sens);

void adcSaveData(ADCSensor* sens);

void gpsSaveData(GPSSensor * sens);

void sdFlush();

void sdMountFailHandler();
#endif /* INC_SDCARD_H_ */

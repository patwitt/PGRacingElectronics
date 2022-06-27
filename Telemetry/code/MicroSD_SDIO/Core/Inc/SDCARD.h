/*
 * SDCARD.h
 *
 *  Created on: 27 cze 2022
 *      Author: Patryk
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include "sensorFunctions.h"
void sdInit(FATFS* fs);
void openAllFiles();
int createHeaders(FIL * file,char * path);

int openFile(FIL * file, char * path, BYTE mode);


void gyroSaveData(GyroSensor* sens);

void mlxSaveData(MLXSensor* mlx);


void sdMountFailHandler();
#endif /* INC_SDCARD_H_ */

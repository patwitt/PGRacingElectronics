/*
 * SDCARD.h
 *
 *  Created on: 27 cze 2022
 *      Author: Patryk
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include "sensorFunctions.h"
void initSDCard(FATFS* fs);

int createHeaders(FIL * file,char * path);

int openFile(FIL * file, char * path, BYTE mode);


void saveGyroData(GyroSensor* sens);

void saveMLXData(MLXSensor* mlx);


void mountFailHandler();
#endif /* INC_SDCARD_H_ */

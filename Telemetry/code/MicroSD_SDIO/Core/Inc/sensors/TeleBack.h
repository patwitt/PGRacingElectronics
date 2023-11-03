/*
 * TeleBack.h
 *
 *  Created on: 10 lis 2022
 *      Author: Patryk
 */

#ifndef INC_SENSORS_TELEBACK_H_
#define INC_SENSORS_TELEBACK_H_

#include "sdcard/sdmmc.h"
#include "ff.h"
#include "stdio.h"
//#include "sensorFunctions.h"



typedef struct TeleBackData_t{
	FIL *File; //Teleback File to write
	char path[20]; // path of file to write;
	int ABSLRData;
	uint8_t ABSLRReady : 1;
	int ABSRRData;
	uint8_t ABSRRReady : 1;
	int DamperLRData;
	uint8_t DamperLRReady : 1;
	int DamperRRData;
	uint8_t DamperRRReady : 1;
	int timestamp;
}TeleBackData;


enum TeleBackFrames{
	DamperLRFrame = 0x702,
	DamperRRFrame = 0x703,
	ABSLRFrame = 0x704,
	ABSRRFrame = 0x705
};
void TeleBackInit(TeleBackData * tele);
int TeleBackAnyDataReady(TeleBackData tele);
#endif /* INC_SENSORS_TELEBACK_H_ */

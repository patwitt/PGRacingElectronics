/* *******ADC SECTION  ********/
#include "sdcard/sdmmc.h"
#include "ff.h"
#include "stdio.h"

#pragma once
typedef enum AdcSensorTypes{
	damper = 0,
	steeringWheel,
	brake
}AdcSensorType;

typedef struct ADCSensor{
	FIL *File; //ADC File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read and write to file
	uint16_t* data;
	int ID;
	AdcSensorType adcType;
	ADC_HandleTypeDef * adc;
	int timeToNextRead;
	int adcChannel;
	int timestamp;
	int sendDataByCanTime;
}ADCSensor;


void adcInit(ADCSensor* sens,ADC_HandleTypeDef * adc,int channel,FIL * f);

void damperInit(ADCSensor* sens,int id,FIL * f);

void steeringInit(ADCSensor* sens);

void adcGetData(ADCSensor * sens);

void adcSendDataByCan(ADCSensor * sens);

void adcSetActiveChannel(ADCSensor* sens);

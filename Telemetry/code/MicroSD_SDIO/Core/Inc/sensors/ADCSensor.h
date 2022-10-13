/* *******ADC SECTION  ********/
#include "sdcard/sdmmc.h"
#include "ff.h"
#include "stdio.h"
#include "sensorFunctions.h"
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
	int timeToNextRead;
	int timeFromLastSuccRead;
	int data;
	int ID;
	AdcSensorType adcType;
	ADC_HandleTypeDef * adc;
	int adcChannel;
}ADCSensor;


void adcInit(ADCSensor* sens,ADC_HandleTypeDef * adc,int channel,FIL * f);

void damperInit(ADCSensor* sens,SENSORS id,FIL * f);

void steeringInit(ADCSensor* sens);

void adcGetData(ADCSensor * sens);

/*
 * handler.c
 *
 *  Created on: 12 sie 2022
 *      Author: Patryk
 */
#include "sensorFunctions.h"
#include "SDCARD.h"

#pragma once

extern GyroSensor gyro;
extern MLXSensor mlxLFSensor;
extern MLXSensor mlxRFSensor;
extern ABSSensor absLFSensor;
extern ABSSensor absRFSensor;
extern GPSSensor gpsSensor;
extern ADCSensor sWheelSensor;

typedef void (*getDataHander)(void*);
typedef struct
{
  SENSORS sensorType;
  void* sensorStruct;
  getDataHander getDataHandler;
  getDataHander saveDataHandler;
  uint8_t dataReady : 1 ;
  uint8_t isActive : 1;

} sensorDataHandler;

#define SENSORS_N 9




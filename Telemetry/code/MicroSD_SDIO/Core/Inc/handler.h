/*
 * handler.c
 *
 *  Created on: 12 sie 2022
 *      Author: Patryk
 */
#include "sensorFunctions.h"

#pragma once

extern IMUInnerSensor IMUInnerSensor;
extern MLXSensor mlxLFSensor;
extern MLXSensor mlxRFSensor;
extern ABSSensor absLFSensor;
extern ABSSensor absRFSensor;
extern GPSSensor gpsSensor;
extern ADCSensor sWheelSensor;
extern ADCSensor damperLFSensor;
extern ADCSensor damperRFSensor;
extern EcumasterData ecuData;
extern TeleBackData teleData;


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






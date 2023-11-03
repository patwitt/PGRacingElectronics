/*
 * sensorFunctions.h
 *
 *  Created on: Jun 17, 2022
 *      Author: Patr
 */

//INCLUDES

#include <sensors/IMU.h>
#include "stdint.h"
#include "sensors/ABS.h"
#include "sensors/ADCSensor.h"
#include "sensors/GPS.h"
#include "sensors/MLX.h"
#include "sensors/TeleBack.h"
#include "can.h"
#include "ecumaster.h"
#include "string.h"
#include "tim.h"

#pragma once

#define SENSORS_N 13

typedef enum{

	GPS = 0,
	IMU,
	MLXLF,
	MLXRF,
	ABSLF,
	ABSRF,
	WHEEL,
	DAMPERLF,
	DAMPERRF,
	ECU,
	TELEBACK,
	DAMPERLR,
	DAMPERRR,
	ABSLR,
	ABSRR

}SENSORS;



typedef enum{
	SENSOR_OFF = 0b000,
	SENSOR_OK = 0b001,
	SENSOR_1ERROR = 0b010,
	SENSOR_2ERROR = 0b011,
	SENSOR_FAIL = 0b100,
	SENSOR_INVALID_DATA = 0b101,
	SENSOR_INIT_FAIL = 0b111,
	SENSOR_ALL_CHECK_TIME = 10000,
}SENSOR_STATUS;

typedef struct {
	union{
		unsigned int status;
		struct {
		unsigned int SDCARD : 3;
		unsigned int GPS : 3;
		unsigned int IMU : 3;
		unsigned int MLXLF : 3;
		unsigned int MLXRF : 3;
		unsigned int VSSLF : 3;
		unsigned int VSSRF : 3;
		unsigned int Steering : 3;
		unsigned int DamperLF : 3;
		unsigned int DamperRF : 3;
		unsigned int TeleBack : 2;
		};
	};

	uint16_t checkTime;

} SensorStatus;
typedef enum SensorsDeafultTimings{
	MLX_DATA_RATE = 1000,
	IMU_DATA_RATE = 10,
	DAMPER_DATA_RATE = 50,
	STEERING_DATA_RATE = 50,
	ECU_DATA_RATE = 100,

	ABS_ZERO_SPEED_TIME = 100,

	GPS_ERROR_TIME = 1000,
	ADC_ERROR_TIME = 1000,


}SensorsDeafultTimings;

void initSensors();

const char* enumToSensor(SENSORS sensor);

int statusToInt();

void printStatusRegister();



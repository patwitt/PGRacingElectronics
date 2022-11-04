/*
 * sensorFunctions.h
 *
 *  Created on: Jun 17, 2022
 *      Author: Patr
 */

//INCLUDES

#include "basicFunctions.h"

#include "i2c.h"


#pragma once
#define DEBUG 1 // Debug writing on/off
#define SENSORS_N 10

typedef enum{

	GPS = 0,
	GYRO,
	MLXLF,
	MLXRF,
	ABSLF,
	ABSRF,
	WHEEL,
	DAMPERLF,
	DAMPERRF,
	ECU

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
		unsigned int GYRO : 3;
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
	GYRO_DATA_RATE = 250,
	DAMPER_DATA_RATE = 50,
	STEERING_DATA_RATE = 50,
	ECU_DATA_RATE = 100,

	ABS_ZERO_SPEED_TIME = 100,

	GPS_ERROR_TIME = 1000,
	ADC_ERROR_TIME = 1000,


}SensorsDeafultTimings;

void initSensors();

void enumToSensor(char *buf,SENSORS sensor);

int statusToInt();

void printStatusRegister();



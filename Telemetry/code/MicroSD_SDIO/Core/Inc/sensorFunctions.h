/*
 * sensorFunctions.h
 *
 *  Created on: Jun 17, 2022
 *      Author: Patryk
 */

//INCLUDES
#include "MPU9250.h"
#include "MLX90640_API.h"
#include "basicFunctions.h"
#include "i2c.h"
#include "sdmmc.h"
#include "ff.h"

#define DEBUG 1 // Debug writing on/off

#define MLXDATARATE 1000 // MLX data read interval in miliseconds
#define GYRODATARATE 250 // Gyro data read interval in milisencods

typedef enum {
	GPS = 0,
	GYRO,
	MLX,
	VSS,
	WHEEL,
	DAMPER,

};

typedef enum{
	SENSOR_OFF = 0b000,
	SENSOR_OK = 0b001,
	SENSOR_1ERROR = 0b010,
	SENSOR_2ERROR = 0b011,
	SENSOR_FAIL = 0b100,
	SENSOR_INVALID_DATA = 0b101,
	SENSOR_INIT_FAIL = 0b111,
	SENSOR_ALL_CHECK_TIME = 10000,
};

typedef struct SensorStatus{
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
	uint16_t checkTime;



} SensorStatus;

void sensorsInit();

/* *******GYRO SECTION  ********/
//GYRO DEFINES
//GYRO STRUCTS

typedef struct imu_9dof
{
	int16_t acc_data[3];
	int16_t mag_data[3];
	int16_t gyro_data[3];
}imu_9dof;

typedef struct imu_9dof_calc
{
	double acc_data_calc[3];
	double gyro_data_calc[3];
}imu_9dof_calc;

typedef struct GYRO{
	FIL *File; //GYRO File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read/write to file
	int saveRate;
	int timeToNextRead;
	imu_9dof_calc data;
	I2C_HandleTypeDef i2c;
}GyroSensor;

//GYRO FUNCS
void gyroInit(GyroSensor * gyro);

void gyroConvertData(struct imu_9dof * input, struct imu_9dof_calc * output);

void gyroGetData(GyroSensor * sens);


/* *******MLX90640 SECTION  ********/
//MLX DEFINES

#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33 // Defult mlx address
#define	 RefreshRate 0x04
#define  TA_SHIFT 8 //Default shift for MLX90640 in open air

#define leftFrontWheelI2C hi2c1
#define rightFrontWheelI2C hi2c2
//MLX STRUCTS
//UNIONS?
typedef struct MLX{
	FIL *File; //mlx File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read/write to file
	int saveRate;
	int timeToNextRead;
	int ID ;
	I2C_HandleTypeDef i2c; // I2C connected to sensor
	paramsMLX90640 mlx90640;
	float data[768]; // data frame array of 768 pixels
	float ambientTemp; // temperature of sensor itself
	float vdd; // Voltage
	float emissivity;

}MLXSensor;




//MLX FUNCS
int mlxInit(MLXSensor *mlx);

//Copy and calcualte data from sensor memory to stm
int mlxGetData(MLXSensor* mlx);

//DEBUG printing on default uart
void mlxPrintData(MLXSensor* mlx);



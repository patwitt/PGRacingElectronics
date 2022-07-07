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




typedef enum {
	GPS = 0,
	GYRO,
	MLXLF,
	MLXRF,
	ABSLF,
	ABSRF,
	WHEEL,
	DAMPERLF,
	DAMPERRF,


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
typedef enum SensorsDeagultTimings{
	MLX_DATA_RATE = 1000,
	GYRO_DATA_RATE = 250,
	DAMPER_DATA_RATE = 50,
	STEERING_DATA_RATE = 50,

	ABS_ZERO_SPEED_TIME = 100,

	GPS_ERROR_TIME = 1000,
	ADC_ERROR_TIME = 1000,

};
void sensorsInit();
/* *******ADC SECTION  ********/
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
/* *******GPS SECTION  ********/


typedef struct GPS{
	FIL *File; //GPS File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read and write to file
	int saveRate;
	char bufor[255];
	char data[255];
	USART_TypeDef uart;
}GPSSensor;
/* *******ABS SECTION  ********/


typedef struct ABS{
	FIL *File; //ABS File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read and write to file
	int saveRate;
	int data;
	int ID;
	//ABS data asynchronous
	int timeToZeroSpeed; //after that time if not new input we assume car is not moving
	TIM_HandleTypeDef * timer;
	int timerChannel;
}ABSSensor;

void absInit(ABSSensor * sens,SENSORS id,TIM_HandleTypeDef * tim,int channel,FIL* f);
/* *******GYRO SECTION  ********/
//GYRO DEFINES
//GYRO STRUCTS

typedef struct gyroData
{
	int16_t acc_data[3];
	int16_t mag_data[3];
	int16_t gyro_data[3];
}gyroData;

typedef struct gyroDataCalc
{
	double acc_data_calc[3];
	double gyro_data_calc[3];
}gyroDataCalculated;

typedef struct GYRO{
	FIL *File; //GYRO File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read/write to file
	int saveRate;
	int timeToNextRead;
	gyroDataCalculated data;
	I2C_HandleTypeDef i2c;
}GyroSensor;

//GYRO FUNCS
void gyroInit(GyroSensor * gyro);

void gyroGetData(GyroSensor * sens);

void gyroConvertData(struct gyroData * input, struct gyroDataCalc * output);
/* *******MLX90640 SECTION  ********/
//MLX DEFINES

#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33 // Default mlx address
#define	 RefreshRate 0x04
#define  TA_SHIFT 8 //Default shift for MLX90640 in open air


//MLX STRUCTS
typedef struct MLX{
	FIL *File; //mlx File to write
	char path[20]; // path of file to write;
	int dataReady; // flag to check if data is ready to read/write to file
	int saveRate;
	int timeToNextRead;
	int ID ;
	I2C_HandleTypeDef * i2c; // I2C connected to sensor
	paramsMLX90640 mlx90640;
	float data[768]; // data frame array of 768 pixels
	float ambientTemp; // temperature of sensor itself
	float vdd; // Voltage
	float emissivity;

}MLXSensor;


//MLX FUNCS
int mlxInit(MLXSensor *mlx,SENSORS id, I2C_HandleTypeDef * i2c,FIL * f);

//Copy and calcualte data from sensor memory to stm
int mlxGetData(MLXSensor* mlx);

//DEBUG printing on default uart
void mlxPrintData(MLXSensor* mlx);



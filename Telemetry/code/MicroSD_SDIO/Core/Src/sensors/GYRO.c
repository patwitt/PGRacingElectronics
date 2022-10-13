/*
 * GYRO.c
 *
 *  Created on: 28 wrz 2022
 *      Author: Patryk
 */
#include "sensors/GYRO.h"

extern RTC_HandleTypeDef hrtc;
extern SensorStatus statusRegister;
/* *******GYRO SECTION  ********/

//GYRO FUNCS
void gyroInit(GyroSensor * sens)
{
	int result = MPU9250_Init();
	if(result == 0)
	{
		statusRegister.GYRO = SENSOR_OK;
	}else{
		statusRegister.GYRO = SENSOR_INIT_FAIL;
	}
	sens->File = (FIL*)malloc(sizeof(FIL));

	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"GYRO%02d%02d.csv",date.Date,date.Month);
	sens->dataReady = 0;
	sens->saveRate = GYRO_DATA_RATE;
	sens->timeToNextRead = GYRO_DATA_RATE;
}
void gyroConvertData(struct gyroData * input, struct gyroDataCalc * output)
{
	output->acc_data_calc[0] = (double) input->acc_data[0] / 16384;
	output->acc_data_calc[1] = (double) input->acc_data[1] / 16384;
	output->acc_data_calc[2] = (double) input->acc_data[2] / 16384;
	output->gyro_data_calc[0] = (double) input->gyro_data[0] * 250 / 32768;
	output->gyro_data_calc[1] = (double) input->gyro_data[1] * 250 / 32768;
	output->gyro_data_calc[2] = (double) input->gyro_data[2] * 250 / 32768;

}

void gyroGetData(GyroSensor * sens)
{
	gyroData imu_9dof_data;
    MPU9250_GetData(imu_9dof_data.acc_data, imu_9dof_data.mag_data, imu_9dof_data.gyro_data);
    gyroConvertData(&imu_9dof_data, &sens->data);
    sens->dataReady = 0;
}

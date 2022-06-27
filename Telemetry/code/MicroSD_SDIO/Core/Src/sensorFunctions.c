/*
 * sensorFunctions.h
 *
 *  Created on: Jun 17, 2022
 *      Author: Patryk
 */

//INCLUDES

#include "sensorFunctions.h"

extern RTC_HandleTypeDef hrtc;
extern SensorStatus statusRegister;


/* *******GYRO SECTION  ********/
GyroSensor gyro;
//GYRO FUNCS
void gyroInit(GyroSensor * gyro)
{
	int result = MPU9250_Init();
	if(result == 0)
	{
		statusRegister.GYRO = SENSOR_OK;
	}else{
		statusRegister.GYRO = SENSOR_INIT_FAIL;
	}
	gyro->File = (FIL*)malloc(sizeof(FIL));

	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(gyro->path,"GYRO%02d%02d.csv",date.Date,date.Month);
	gyro->dataReady = 0;
	gyro->saveRate = GYRODATARATE;
	gyro->timeToNextRead = GYRODATARATE;
}
void imu_9dof_convert(struct imu_9dof * input, struct imu_9dof_calc * output)
{
	output->acc_data_calc[0] = (double) input->acc_data[0] / 16384;
	output->acc_data_calc[1] = (double) input->acc_data[1] / 16384;
	output->acc_data_calc[2] = (double) input->acc_data[2] / 16384;
	output->gyro_data_calc[0] = (double) input->gyro_data[0] * 250 / 32768;
	output->gyro_data_calc[1] = (double) input->gyro_data[1] * 250 / 32768;
	output->gyro_data_calc[2] = (double) input->gyro_data[2] * 250 / 32768;

}

void imu_9dof_get_data(struct imu_9dof_calc * imu_9dof_calculated)
{
	imu_9dof imu_9dof_data;
    MPU9250_GetData(imu_9dof_data.acc_data, imu_9dof_data.mag_data, imu_9dof_data.gyro_data);
    imu_9dof_convert(&imu_9dof_data, imu_9dof_calculated);
    gyro.dataReady = 0;
}


/* *******MLX90640 SECTION  ********/

MLXSensor MLXLF;
MLXSensor MLXRF;

//MLX FUNCS
int mlxInit(MLXSensor *mlx)
{
		static uint16_t eeMLX90640[832];
		static int ID = 1;
		RTC_DateTypeDef date;
		HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

		sprintf(mlx->path,"MLX%02d%02d.csv",date.Date,date.Month);
		mlx->timeToNextRead = MLXDATARATE;
		mlx->dataReady = 0;
	  	mlx->emissivity = 0.95;
	  	if(ID == 0)
	  	{
	  		mlx->ID = ID;
			ID++;
	  	}



		MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate);
	  	MLX90640_SetChessMode(MLX90640_ADDR);
	    int status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640);
	    if (status != 0){
	    	if(ID == 1)
	    	{
	    		statusRegister.MLXLF = SENSOR_INIT_FAIL;
	    	}else if(ID == 2)
	    	{
	    		statusRegister.MLXRF = SENSOR_INIT_FAIL;
	    	}
	    	return status;
	    }
	    status = MLX90640_ExtractParameters(eeMLX90640, &mlx->mlx90640);
	    if (status != 0){
	    	 if(mlx->ID == 1)
	    	 {
	    	    statusRegister.MLXLF = SENSOR_INIT_FAIL;
	    	 }else if(mlx->ID == 2)
	    	 {
	    	    statusRegister.MLXRF = SENSOR_INIT_FAIL;
	    	 }
	    	 return status;
	   }


	    if(mlx->ID == 1)
	    {
	    	mlx->File = (FIL*)malloc(sizeof(FIL));
	    	mlx->i2c = leftFrontWheelI2C;
	    	statusRegister.MLXLF = SENSOR_OK;
	    }else if(mlx->ID == 2)
	    {
	    	mlx->i2c = rightFrontWheelI2C;
	    	mlx->File = MLXLF.File;
	    	statusRegister.MLXRF = SENSOR_OK;
	    }

	    return 0;
}

//Copy and calcualte data from sensor memory to stm
int mlxGetData(MLXSensor* mlx){
	uint16_t frame[832];
	int status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
	if (status < 0)
	{
		if(DEBUG)
			printf("Error while getting frame 1: %d\n",status);
		return status;
	}

	mlx->vdd = MLX90640_GetVdd(frame, &mlx->mlx90640);
	mlx->ambientTemp = MLX90640_GetTa(frame, &mlx->mlx90640) - TA_SHIFT;

	MLX90640_CalculateTo(frame, &mlx->mlx90640, mlx->emissivity , mlx->ambientTemp, mlx->data);
	status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
	if(status < 0)
	{
		if(DEBUG)
			printf("Error while getting frame 2: %d\n",status);
		return status;
	}
	mlx->dataReady = 1;
	MLX90640_CalculateTo(frame, &mlx->mlx90640, mlx->emissivity , mlx->ambientTemp, mlx->data);
	return 0;
}

//DEBUG printing on default uart
void mlxPrintData(MLXSensor* mlx)
{
	for(int i = 0; i < 768; i++){
		if(i%32 == 0 && i != 0){
			printf("\r\n");
		}
		printf("%2.2f ",mlx->data[i]);
	}
}

/*
 * MLX.c
 *
 *  Created on: 28 wrz 2022
 *      Author: Patryk
 */

#include "sensors/MLX.h"
/* *******MLX90640 SECTION  ********/


extern RTC_HandleTypeDef hrtc;
extern SensorStatus statusRegister;
//MLX FUNCS
int mlxInit(MLXSensor *mlx,SENSORS id, I2C_HandleTypeDef * i2c,FIL * f)
{
		uint16_t eeMLX90640[832];
		if(f == 0)
		{
			mlx->File = (FIL*)malloc(sizeof(FIL));
		}else
		{
			mlx->File = f;
		}
		RTC_DateTypeDef date;
		HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
		sprintf(mlx->path,"MLX%02d%02d.csv",date.Date,date.Month);
		mlx->timeToNextRead = MLX_DATA_RATE;
		mlx->dataReady = 0;
	  	mlx->emissivity = 0.95;
	  	mlx->ID = id;
		MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate,mlx->i2c);
	  	MLX90640_SetChessMode(MLX90640_ADDR,mlx->i2c);
	    int status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640,mlx->i2c);
	    status = status | MLX90640_ExtractParameters(eeMLX90640, &mlx->mlx90640);
	    if (status != 0){
	    	 if(mlx->ID == MLXLF)
	    	 {
	    	    statusRegister.MLXLF = SENSOR_INIT_FAIL;
	    	 }else if(mlx->ID == MLXRF)
	    	 {
	    	    statusRegister.MLXRF = SENSOR_INIT_FAIL;
	    	 }
	    	 return status;
	   }
	    mlx->i2c = i2c;

	    if(mlx->ID == MLXLF)
	    {
	    	statusRegister.MLXLF = SENSOR_OK;
	    }else if(mlx->ID == MLXRF)
	    {
	    	statusRegister.MLXRF = SENSOR_OK;
	    }


	    return 0;
}

//Copy and calcualte data from sensor memory to stm
int mlxGetData(MLXSensor* mlx){
	uint16_t frame[832];
	int status = MLX90640_GetFrameData(MLX90640_ADDR, frame,mlx->i2c);
	if (status < 0)
	{
		return status;
	}

	mlx->vdd = MLX90640_GetVdd(frame, &mlx->mlx90640);
	mlx->ambientTemp = MLX90640_GetTa(frame, &mlx->mlx90640) - TA_SHIFT;

	MLX90640_CalculateTo(frame, &mlx->mlx90640, mlx->emissivity , mlx->ambientTemp, mlx->data);
	status = MLX90640_GetFrameData(MLX90640_ADDR, frame,mlx->i2c);
	if(status < 0)
	{
		return status;
	}
	//mlx->dataReady = 1;
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

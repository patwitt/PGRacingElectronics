/*
 * SDCARD.c
 *
 *  Created on: 27 cze 2022
 *      Author: Patryk
 */
#include "SDCARD.h"
#include<stdio.h>
#define FILE_DEFAULT_MODE FA_WRITE | FA_OPEN_APPEND

extern MLXSensor MLXLF;
extern MLXSensor MLXRF;
extern GyroSensor gyro;
extern SensorStatus statusRegister;

void sdInit(FATFS* fs)
{
	  if (f_mount(fs, "", 1) == FR_OK)
	  {
		  statusRegister.SDCARD = SENSOR_OK;
		  if (DEBUG)
			  printf("SDCard mounting success!\n");

	  }else
	  {
		  statusRegister.SDCARD = SENSOR_INIT_FAIL;
		  sdMountFailHandler();
	  }
}
void openAllFiles()
{
	if(statusRegister.SDCARD == SENSOR_OK)
	{
		if(statusRegister.GYRO == SENSOR_OK){
			openFile(gyro.File, gyro.path, FILE_DEFAULT_MODE);
		}

	}
}
int createHeaders(FIL * file,char * path)
{
	FRESULT fres;
	UINT bytesWritten = 0;

	fres = f_write(file,"timestamp,",strlen("timestamp,"),&bytesWritten);
	if(fres != FR_OK){
		printf("Error while creating %s header",path);
		return -1;
	}
	if(strstr(path,"GYRO.csv") == 0){
		fres = f_write(file, "gyro_x,gyro_y,gyro_z,acc_x,acc_y,acc_z\r\n", strlen("gyro_x,gyro_y,gyro_z,acc_x,acc_y,acc_z\r\n"), &bytesWritten);

	}else if(strstr(path,"MLX.csv") == 0){
		char headerData[25];
		fres =  f_write(file, "ID", strlen("ID"), &bytesWritten);
		for(int i=0;i<784;i++){
			sprintf(headerData,"float_%d,", i);
			fres = fres | f_write(file, headerData, strlen(headerData), &bytesWritten);
			sprintf(headerData,"id,");
			fres =  fres | f_write(file, headerData, strlen(headerData), &bytesWritten);
		}

	}else
	{
		return -2;
	}
	if(fres != FR_OK){
		printf("Error while creating %s header\n",path);
		return -1;
	}
	fres =  f_write(file, "\n", strlen("\n"), &bytesWritten);
	return 1;

}

int openFile(FIL * file, char * path, BYTE mode)
{
	FILINFO fInfo;
	FRESULT fres = f_stat(path, &fInfo);

	if(fres == FR_OK)
	{

		fres = f_open(file, _TEXT(path), mode);
		if(fres == FR_OK)
		{
			printf("Opening file: %s succeeded\n", path);
		}else
		{
			return -1;
		}
	}else if(fres == FR_NO_FILE)
	{
		fres = f_open(file, _TEXT(path), mode);
		if(fres == FR_OK)
		{
			int res = createHeaders(file,path);
			if(res != 1)
			{
				return res;
			}

			printf("No file: %s, created new\n", path);

		}else
		{
			return -1;
		}
	}
	f_sync(file);
	return 1;


}

void gyroSaveData(GyroSensor* sens)
{
	char dataBuffer[255];
	int writedBytes;
	//Save time stamp
	sprintf(dataBuffer, "%d,", HAL_GetTick());
	f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	for (int i = 0; i < 3; i++)
	{
		sprintf(dataBuffer, "%f,", sens->data.gyro_data_calc[i]);
		f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);

	}
	for (int i = 0; i < 3; i++)
	{
		sprintf(dataBuffer, "%f,", sens->data.acc_data_calc[i]);
		f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	}

	sprintf(dataBuffer, "\r\n ");

	f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	f_sync(sens->File);

}
void mlxSaveData(MLXSensor* mlx)
{
	char dataBuffer[255];
	int writedBytes;
	//Save time stamp and mlx ID
	sprintf(dataBuffer, "%d,%d", HAL_GetTick(),mlx->ID);
	int fres = f_write(mlx->File, dataBuffer, strlen(dataBuffer), &writedBytes);

	for(int i=0;i<784;i++)
	{
		sprintf(dataBuffer,"%2.2f,", mlx->data[i]);
		fres = fres | f_write(mlx->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	}

	sprintf(dataBuffer, "\r\n ");
	f_write(mlx->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	f_sync(mlx->File);
}

void sdMountFailHandler()
{
	printf("SDCard mount failed\n");
	while(1)
	{

	}
}

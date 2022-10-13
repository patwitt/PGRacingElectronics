/*
 * SDCARD.c
 *
 *  Created on: 27 cze 2022
 *      Author: Patryk
 */
#include "sdcard/SDCARD.h"

#include<stdio.h>
#include "ecumaster.h"
#include "string.h"
#define FILE_DEFAULT_MODE FA_WRITE | FA_OPEN_APPEND

extern GyroSensor gyro;
extern MLXSensor mlxLFSensor;
extern MLXSensor mlxRFSensor;
extern ABSSensor absLFSensor;
extern ABSSensor absRFSensor;
extern ADCSensor damperLFSensor;
extern ADCSensor damperRFSensor;
extern GPSSensor gpsSensor;
extern SensorStatus statusRegister;


const char GYRO_HEADER[] = "gyro_x,gyro_y,gyro_z,acc_x,acc_y,acc_z\r\n";


void sdDeInit(FATFS* fs)
{
	  if (f_mount(0,"",0) == FR_OK)
	  {
		  statusRegister.SDCARD = SENSOR_OFF;
	  }
}

void sdInit(FATFS* fs)
{
	  if (f_mount(fs, "", 1) == FR_OK)
	  {
		  statusRegister.SDCARD = SENSOR_OK;
		  if(DEBUG)
			  printf("SDCard mounting success!\n");

	  }else
	  {
		  statusRegister.SDCARD = SENSOR_INIT_FAIL;
		  printf("SDCard mounting fail!\n");

		  //sdMountFailHandler();
	  }
}
FIL *EcuFile;
FIL *AlertFile;
char ecuPath[] = "EcuData.csv";
void openAllFiles()
{

	if(statusRegister.SDCARD == SENSOR_OK)
	{
		EcuFile = (FIL*)malloc(sizeof(FIL));
		openFile(gpsSensor.File,gpsSensor.path,FILE_DEFAULT_MODE);
		//AlertFile = (FIL*)malloc(sizeof(FIL));
		//openFile(AlertFile,"Alert.txt",FILE_DEFAULT_MODE);
		openFile(EcuFile,ecuPath,FILE_DEFAULT_MODE);
		if(statusRegister.GYRO == SENSOR_OK){
			openFile(gyro.File, gyro.path, FILE_DEFAULT_MODE);
		}
		if(statusRegister.MLXLF == SENSOR_OK){
			openFile(mlxLFSensor.File, mlxLFSensor.path, FILE_DEFAULT_MODE);
		}
		if(statusRegister.VSSLF == SENSOR_OK){
			openFile(absLFSensor.File, absLFSensor.path, FILE_DEFAULT_MODE);
		}
		if(statusRegister.DamperLF == SENSOR_OK){
			openFile(damperLFSensor.File, damperLFSensor.path, FILE_DEFAULT_MODE);
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
		return SD_WRITE_ERROR;
	}

	if(strstr(path,"GYRO") != NULL){
		fres = f_write(file, GYRO_HEADER, strlen(GYRO_HEADER), &bytesWritten);
	}else if(strstr(path,"MLX")!= NULL){
		char headerText[25];
		fres =  f_write(file, "ID,", strlen("ID,"), &bytesWritten);
		for(int i=0;i<784;i++){
			sprintf(headerText,"float_%d,", i);
			fres = fres | f_write(file, headerText, strlen(headerText), &bytesWritten);
			sprintf(headerText,"id,");
			fres =  fres | f_write(file, headerText, strlen(headerText), &bytesWritten);
		}

	}else if(strstr(path,"ABS")!= NULL){
		fres = f_write(file, "ID,speed\r\n", strlen("ID,speed\r\n"), &bytesWritten);
	}else if(strstr(path,"DAMP")!= NULL){
		fres = f_write(file, "ID,delta\r\n", strlen("ID,delta\r\n"), &bytesWritten);
	}else if(strstr(path,"WHEEL")!= NULL){
		fres = f_write(file, "ID,angle\r\n", strlen("ID,angle\r\n"), &bytesWritten);
	}else if(strstr(path,"GPS")!= NULL){
		fres = f_write(file, "LOG,utc,pos status,lat,lat dir,lon,lon dir,speed,,track,date,,mag var,var dir,mode ind,chs,ter\r\n", strlen("LOG,utc,pos status,lat,lat dir,lon,lon dir,speed,,track,date,,mag var,var dir,mode ind,chs,ter\r\n"), &bytesWritten);
	}else
	{
		return WRONG_PARAMETER;
	}

	if(fres != FR_OK){
		printf("Error while creating %s header\n",path);
		return SD_WRITE_ERROR;
	}
	fres =  f_write(file, "\n", strlen("\n"), &bytesWritten);
	return SD_OK;

}


int openFile(FIL * file, char * path, BYTE mode)
{
	FILINFO fInfo;
	FRESULT fres = f_stat(path, &fInfo);

	if(fres == FR_OK)
	{
		fres = f_open(file, _TEXT(path), mode);
		int bw;
		fres |= f_write(file,"---------------Restart------------------\r\n",strlen("---------------Restart------------------\r\n"),&bw);
		if(fres == FR_OK)
		{
			printf("Opening file: %s succeeded\n", path);
		}else
		{
			return SD_READ_ERROR;
		}

	}else if(fres == FR_NO_FILE)
	{
		fres = f_open(file, _TEXT(path), mode);
		if(fres == FR_OK)
		{
			int res = createHeaders(file,path);
			if(res != SD_OK)
			{
				return res;
			}

			printf("No file: %s, created new\n", path);

		}else
		{
			return SD_READ_ERROR;
		}
	}
	f_sync(file);
	return SD_OK;


}
void ecuSaveData(EcumasterData ecu){
	int bw;
	char dataBuffer[255];
	sprintf(dataBuffer, "%u,", HAL_GetTick());
	f_write(EcuFile, dataBuffer, strlen(dataBuffer), &bw);
	 for(int i = 0; i < sizeof(ecu); i++)
	    {
	        sprintf(dataBuffer,"%X",((char*)&ecu)[i]);
	        f_write(EcuFile, dataBuffer, strlen(dataBuffer), &bw);
	    }
	//int res = f_write(EcuFile, &ecu, sizeof(ecu), &bw);
	f_write(EcuFile, "\r\n", 2, &bw);
	f_sync(EcuFile);
}
void gpsSaveData(GPSSensor * sens)
{
	char dataBuffer[255];
	int writedBytes;
	FRESULT status = 0;
	//Save time stamp
	sprintf(dataBuffer, "%d,", HAL_GetTick());
	status = f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	status = status | f_write(sens->File,sens->data,strlen(sens->data),&writedBytes);
	sens->dataReady = 0;
	f_sync(sens->File);
}
void gyroSaveData(GyroSensor* sens)
{
	char dataBuffer[255];
	int writedBytes;
	FRESULT status = 0;
	//Save time stamp
	sprintf(dataBuffer, "%d,", HAL_GetTick());
	status = f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	for (int i = 0; i < 3; i++)
	{
		sprintf(dataBuffer, "%f,", sens->data.gyro_data_calc[i]);
		status = status | f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);

	}
	for (int i = 0; i < 3; i++)
	{
		sprintf(dataBuffer, "%f,", sens->data.acc_data_calc[i]);
		status = status | f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	}

	sprintf(dataBuffer, "\r\n ");

	status = status | f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	if(status != 0 && statusRegister.SDCARD < SENSOR_FAIL && statusRegister.SDCARD > SENSOR_OFF)
	{
		statusRegister.SDCARD += 1;
	}
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

void absSaveData(ABSSensor * sens)
{
	char dataBuffer[255];
	int writedBytes;
	float calcData = absCalculate(sens->data);
	sprintf(dataBuffer, "%d,%d,%f\r\n", HAL_GetTick(),sens->ID,calcData);
	int fres = f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	f_sync(sens->File);
}

void adcSaveData(ADCSensor * sens)
{
	char dataBuffer[255];
	int writedBytes;
	sprintf(dataBuffer, "%d,%d,%d\r\n", HAL_GetTick(), sens->ID,sens->data);
	int fres = f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	f_sync(sens->File);
}

void sdMountFailHandler()
{
	printf("SDCard mount failed\n");
	while(1)
	{

	}
}

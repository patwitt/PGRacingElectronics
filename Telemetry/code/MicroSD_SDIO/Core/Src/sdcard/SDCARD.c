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
FIL *StatsFile;
char ecuPath[] = "EcuData.csv";
void openAllFiles()
{

	if(statusRegister.SDCARD == SENSOR_OK)
	{
		openFile(gpsSensor.File,gpsSensor.path,FILE_DEFAULT_MODE);
		EcuFile = (FIL*)malloc(sizeof(FIL));

		StatsFile = (FIL*)malloc(sizeof(FIL));
		openFile(StatsFile,"Stats.txt",FILE_DEFAULT_MODE);
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
		UINT bw;
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

void statsSave(int operation, int time, int sensor){
	UINT bw;
	char dataBuffer[255];
	char sensorBuffer[15];
	enumToSensor(sensorBuffer, sensor);
	int duration = measureTime(time);
	int dataLength = sprintf(dataBuffer,"[%d] Operation: %d from Sensors: %s took time: %d\r\n",getSeconds(),operation,sensorBuffer,duration);
	f_write(StatsFile, dataBuffer,dataLength,&bw);
	f_sync(StatsFile);
}
void ecuSaveData(EcumasterData ecu){
	UINT bw;
	char dataBuffer[255];
	int dataLength = sprintf(dataBuffer, "%lu,", getSeconds());
	f_write(EcuFile, dataBuffer, dataLength, &bw);
	for(int i = 0; i < sizeof(ecu); i++)
	 {
		dataLength = sprintf(dataBuffer,"%X",((char*)&ecu)[i]);
	        f_write(EcuFile, dataBuffer, dataLength, &bw);
	 }
	//int res = f_write(EcuFile, &ecu, sizeof(ecu), &bw);
	f_write(EcuFile, "\r\n", 2, &bw);
	//f_sync(EcuFile);
}
void gpsSaveData(GPSSensor * sens)
{
	char dataBuffer[255];
	UINT writedBytes;
	FRESULT status = 0;
	//Save time stamp
	int dataLength = sprintf(dataBuffer, "%lu,", getSeconds());
	status = f_write(sens->File, dataBuffer, dataLength, &writedBytes);
	status = status | f_write(sens->File,sens->data,strlen(sens->data),&writedBytes);
	printf("%s,$s\r\n",dataBuffer,sens->data);
	sens->dataReady = 0;
	//f_sync(sens->File);
}
void gyroSaveData(GyroSensor* sens)
{
	char dataBuffer[255];
	UINT writedBytes;
	FRESULT status = 0;
	//Save time stamp
	int dataLength = sprintf(dataBuffer, "%lu,", getSeconds());
	status = f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);
	printf(dataBuffer);
	for (int i = 0; i < 3; i++)
	{
		dataLength = sprintf(dataBuffer, "%f,", sens->data.gyro_data_calc[i]);
		status = status | f_write(sens->File, dataBuffer, dataLength, &writedBytes);
		printf(dataBuffer);

	}
	for (int i = 0; i < 3; i++)
	{
		dataLength = sprintf(dataBuffer, "%f,", sens->data.acc_data_calc[i]);
		status = status | f_write(sens->File, dataBuffer, dataLength, &writedBytes);
		printf(dataBuffer);
	}

	sprintf(dataBuffer, "\r\n ");
	printf(dataBuffer);
	status = status | f_write(sens->File, dataBuffer, strlen(dataBuffer), &writedBytes);

//	f_sync(sens->File);

}

void mlxSaveData(MLXSensor* mlx)
{
	char dataBuffer[255];
	UINT writedBytes;
	//Save time stamp and mlx ID
	int dataLength = sprintf(dataBuffer, "%lu,%d", getSeconds(),mlx->ID);
	int fres = f_write(mlx->File, dataBuffer, dataLength, &writedBytes);

	for(int i=0;i<784;i++)
	{
		dataLength =sprintf(dataBuffer,"%2.2f,", mlx->data[i]);
		fres = fres | f_write(mlx->File, dataBuffer, dataLength, &writedBytes);
	}

	sprintf(dataBuffer, "\r\n ");
	f_write(mlx->File, dataBuffer, 2, &writedBytes);
	//f_sync(mlx->File);
}
extern CAN_HandleTypeDef hcan2;

void absSaveData(ABSSensor * sens)
{
	char dataBuffer[255];
	UINT writedBytes;
	//float calcData = absCalculate(sens->data);
	int dataLength = sprintf(dataBuffer, "%lu,%d,%f\r\n", getSeconds(),sens->ID,sens->data);

	f_write(sens->File, dataBuffer, dataLength, &writedBytes);
	printf(dataBuffer);

	//f_sync(sens->File);
}

void adcSaveData(ADCSensor * sens)
{
	char dataBuffer[255];
	UINT writedBytes;
	int dataLength = sprintf(dataBuffer, "%lu,%d,%d\r\n", getSeconds(), sens->ID,sens->data);
	f_write(sens->File, dataBuffer, dataLength, &writedBytes);
	//f_sync(sens->File);
	printf(dataBuffer);
}
void sdFlush(){
	f_sync(gyro.File);
	f_sync(absLFSensor.File);
	f_sync(gpsSensor.File);
	f_sync(damperLFSensor.File);
	f_sync(EcuFile);
}
void sdMountFailHandler()
{
	printf("SDCard mount failed\n");
	while(1)
	{

	}
}

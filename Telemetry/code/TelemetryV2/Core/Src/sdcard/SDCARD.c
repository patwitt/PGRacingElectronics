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
#include "sensors/TeleBack.h"
#include "handler.h"
#define FILE_DEFAULT_MODE FA_WRITE | FA_OPEN_APPEND

extern IMUSensor IMUInnerSensor;
extern MLXSensor mlxLFSensor;
extern MLXSensor mlxRFSensor;
extern ABSSensor absLFSensor;
extern ABSSensor absRFSensor;
extern ADCSensor damperLFSensor;
extern ADCSensor damperRFSensor;
extern GPSSensor gpsSensor;
extern TeleBackData teleData;
extern SensorStatus statusRegister;
extern sensorDataHandler _dataHandler[];
extern RTC_HandleTypeDef hrtc;
const char IMU_HEADER[] = "gyro_x,gyro_y,gyro_z,acc_x,acc_y,acc_z\r\n";


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
			#if DEBUG
			  printf("SDCard mounting success!\n");
			#endif

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
		if(_dataHandler[GPS].isActive){
			openFile(gpsSensor.File,gpsSensor.path,FILE_DEFAULT_MODE);
		}
		EcuFile = (FIL*)malloc(sizeof(FIL));
		openFile(EcuFile,ecuPath,FILE_DEFAULT_MODE);
		if(_dataHandler[TELEBACK].isActive){
			openFile(teleData.File,teleData.path,FILE_DEFAULT_MODE);
		}

		openFile(IMUInnerSensor.File, IMUInnerSensor.path, FILE_DEFAULT_MODE);

		if(statusRegister.MLXLF == SENSOR_OK){
			openFile(mlxLFSensor.File, mlxLFSensor.path, FILE_DEFAULT_MODE);
		}
		if(statusRegister.VSSLF == SENSOR_OK){
			openFile(absLFSensor.File, absLFSensor.path, FILE_DEFAULT_MODE);
		}
		if(statusRegister.DamperLF == SENSOR_OK){
			openFile(damperLFSensor.File, damperLFSensor.path, FILE_DEFAULT_MODE);
		}
		StatsFile = (FIL*)malloc(sizeof(FIL));
		openFile(StatsFile,"Stats.txt",FILE_DEFAULT_MODE);

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

	if(strstr(path,"IMU") != NULL){
		fres = f_write(file, IMU_HEADER, strlen(IMU_HEADER), &bytesWritten);
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
	char newFileName[40];
	char originalPath[20];
	strcpy(originalPath,path);
	int i = 1;
	originalPath[strlen(path)-4] = 0;
	while(fres == FR_OK){

        sprintf(newFileName,"%s-%d.csv",originalPath,i);
		fres = f_stat(_TEXT(newFileName), &fInfo);
		i++;
	}
	if(i > 1)
		sprintf(path,"%s",newFileName);
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

void statsSave2(int operation, int time){
	UINT bw;
	char dataBuffer[255];
	int duration = measureTime(time);
	if(duration > 50){
		int a = 5;
	}
	int dataLength = sprintf(dataBuffer,"[%d] Operation: %d took time: %d\r\n",getSeconds(),operation,duration);
	f_write(StatsFile, dataBuffer,dataLength,&bw);

}
void statsSave(int operation, int time, int sensor){
	int duration = measureTime(time);
	UINT bw;
	char dataBuffer[255];
	const char * sensorBuffer;
	int dataLength;
	sensorBuffer = enumToSensor(sensor);
	if(sensorBuffer[0] != 0){
		dataLength = sprintf(dataBuffer,"[%d] Operation: %d from Sensors: %s took time: %d\r\n",getSeconds(),operation,sensorBuffer,duration);
	}else
	{
		dataLength = sprintf(dataBuffer,"[%d] Operation: %d from Sensors: %d took time: %d\r\n",getSeconds(),operation,sensor,duration);
	}

	f_write(StatsFile, dataBuffer,dataLength,&bw);
	f_sync(StatsFile);
}
void ecuSaveData(EcumasterData ecu){
	UINT bw;
	char dataBuffer[255];
	int dataLength = sprintf(dataBuffer, "%d,", HAL_GetTick());
	f_write(EcuFile, dataBuffer, dataLength, &bw);
	f_write(EcuFile, &ecu, sizeof(ecu), &bw);
	//int res = f_write(EcuFile, &ecu, sizeof(ecu), &bw);
	f_write(EcuFile, "\r\n", 2, &bw);
	//f_sync(EcuFile);
}

void telebackSaveData(TeleBackData * teleBack){
	UINT bw;
	char dataBuffer[255];
	int dataLength;
	if(teleBack->ABSLRReady){
		dataLength = sprintf(dataBuffer, "%d,%d,%d\r\n", getSeconds(),ABSLR,teleBack->ABSLRData);
		f_write(teleBack->File, dataBuffer, dataLength, &bw);
		teleBack->ABSLRReady = 0;
	}
	if(teleBack->ABSRRReady){
		dataLength = sprintf(dataBuffer, "%d,%d,%d\r\n", getSeconds(),ABSRR,teleBack->ABSRRData);
		f_write(teleBack->File, dataBuffer, dataLength, &bw);
		teleBack->ABSRRReady = 0;
	}
	if(teleBack->DamperLRReady){
		dataLength = sprintf(dataBuffer, "%d,%d,%d\r\n", getSeconds(),DAMPERLR,teleBack->DamperLRData);
		f_write(teleBack->File, dataBuffer, dataLength, &bw);
		teleBack->DamperLRReady = 0;
	}
	if(teleBack->DamperRRReady){
		dataLength = sprintf(dataBuffer, "%d,%d,%d\r\n", getSeconds(),DAMPERRR,teleBack->DamperRRData);
		f_write(teleBack->File, dataBuffer, dataLength, &bw);
		teleBack->DamperRRReady = 0;
	}

}
void gpsSaveData(GPSSensor * sens)
{
	char dataBuffer[255];
	UINT writedBytes;
	FRESULT status = 0;
	//Save time stamp
	char timestamp[25];
	getUTCTimeFormat(timestamp);
	int dataLength = sprintf(dataBuffer, "%s,", timestamp);
	status = f_write(sens->File, dataBuffer, dataLength, &writedBytes);
	status = status | f_write(sens->File,sens->data,strlen(sens->data),&writedBytes);
	//printf("%s",sens->data);
}
void IMUSaveData(IMUSensor* sens)
{
	char dataBuffer[255];
	UINT writedBytes;
	FRESULT status = 0;
	//Save time stamp and id
	char timestamp[25];
	getUTCTimeFormat(timestamp);
	int dataLength = sprintf(dataBuffer, "%s",timestamp);
	status = f_write(sens->File, dataBuffer, dataLength, &writedBytes);
	//printf(dataBuffer);
	for (int i = 0; i < 3; i++)
	{
		dataLength = sprintf(dataBuffer, ",%f", sens->data.gyro_data_calc[i]);
		status = status | f_write(sens->File, dataBuffer, dataLength, &writedBytes);
		//printf(dataBuffer);

	}
	for (int i = 0; i < 3; i++)
	{
		dataLength = sprintf(dataBuffer, ",%f", sens->data.acc_data_calc[i]);
		status = status | f_write(sens->File, dataBuffer, dataLength, &writedBytes);
		//printf(dataBuffer);
	}

	dataLength = sprintf(dataBuffer, "\r\n");
	//printf(dataBuffer);
	status = status | f_write(sens->File, dataBuffer, dataLength, &writedBytes);

}

void mlxSaveData(MLXSensor* mlx)
{
	char dataBuffer[255];
	UINT writedBytes;
	//Save time stamp and mlx ID
	int dataLength = sprintf(dataBuffer, "%d,%d", mlx->timestamp,mlx->ID);
	int fres = f_write(mlx->File, dataBuffer, dataLength, &writedBytes);

	for(int i=0;i<784;i++)
	{
		dataLength = sprintf(dataBuffer,"%2.2f,", mlx->data[i]);
		fres = fres | f_write(mlx->File, dataBuffer, dataLength, &writedBytes);
	}

	sprintf(dataBuffer, "\r\n ");
	f_write(mlx->File, dataBuffer, 2, &writedBytes);
	f_sync(mlx->File);
}


void absSaveData(ABSSensor * sens)
{
	char dataBuffer[100];
	UINT writedBytes;
	int dataLength = sprintf(dataBuffer, "%d,%d,%f\r\n", sens->timestamp,sens->ID,sens->data);
	//printf(dataBuffer);
	f_write(sens->File, dataBuffer, dataLength, &writedBytes);

}

void adcSaveData(ADCSensor * sens)
{
	char dataBuffer[100];
	UINT writedBytes;
	//char timestamp[25];
	//getUTCTimeFormat(timestamp);
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	int mili = ((time.SecondFraction-time.SubSeconds)/((float)time.SecondFraction+1) * 1000);
	int dataLength = sprintf(dataBuffer,"%02d:%02d:%02d:%03d,%d,%d\r\n", time.Hours, time.Minutes, time.Seconds,mili, sens->ID,*(sens->data));
	f_write(sens->File, dataBuffer, dataLength, &writedBytes);
	//printf(dataBuffer);
}
void sdFlush(){
	if(_dataHandler[GPS].isActive)
	f_sync(IMUInnerSensor.File);
	if(_dataHandler[ABSLF].isActive)
	f_sync(absLFSensor.File);
	if(_dataHandler[GPS].isActive)
	f_sync(gpsSensor.File);
	if(_dataHandler[DAMPERLF].isActive)
	f_sync(damperLFSensor.File);
	if(_dataHandler[TELEBACK].isActive)
	f_sync(teleData.File);
	f_sync(EcuFile);
}
void sdMountFailHandler()
{
	printf("SDCard mount failed\n");
	while(1)
	{

	}
}

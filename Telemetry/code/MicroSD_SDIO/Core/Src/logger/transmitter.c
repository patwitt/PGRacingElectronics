/*
 * transmitter.c
 *
 *  Created on: 20 sie 2022
 *      Author: Patryk
 */
#include "logger/transmitter.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "basicFunctions.h"
#pragma once
extern int packetsSend;
extern int saveAlert;
ecuLogRegister _ecuLog = {
	.cltOn = 1,
	.oilTempOn = 1,
	.battOn = 1,
	.oilPressOn =1,
	.BurnedFuelOn =1,
};

void parseCommand(char * command){
	if(strstr(command,"ECUSTATUS:")){
		   char * p = strtok(command, ":");
		   p = strtok(NULL, ":");
		   if(p){
			  _ecuLog.flags = atoi(p);
		   }

	}else if(strstr(command,"ALERT")){
		saveAlert = 1;
	}
}
void sendEcuLogs(EcumasterData EcuData)
{
	HAL_UART_Transmit(&huart3, "ECU DATA: ", strlen("ECU DATA: "), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart3, &EcuData, sizeof(EcuData), HAL_MAX_DELAY);
	packetsSend++;
	int tick = HAL_GetTick();

	//printf("[%d], %s\n",packetsSend, EcuData);
	printf("\n");
	if(_ecuLog.battOn){

		double bat = ((double)EcuData.batt*(0.027));
		printf("[%d] BATT:       %.2f\n",tick, bat);
	}
	if(_ecuLog.tpsOn){
		printf("[%d] TPS:  %d\n",tick, EcuData.tps);
	}
	if(_ecuLog.iatOn){
			printf("[%d] IAT:  %d\n",tick, EcuData.iat);
	}
	if(_ecuLog.mapOn){
			printf("[%d] MAP:  %d\n",tick, EcuData.map);
	}
	if(_ecuLog.injPWOn){
			printf("[%d] injPW:  %d\n",tick, EcuData.injPW);
	}
	if(_ecuLog.speedOn){
			printf("[%d] SPEED:  %d\n",tick, EcuData.speed);
	}
	if(_ecuLog.baroOn){
			printf("[%d] BARO:  %d\n",tick, EcuData.baro);
	}
	if(_ecuLog.oilTempOn){
			printf("[%d] OIL TEMP:  %d\n",tick, EcuData.oilTemp);
	}
	if(_ecuLog.oilPressOn){
			printf("[%d] OIL PRESS:  %d\n",tick, EcuData.oilPress);
	}
	if(_ecuLog.cltOn){
			printf("[%d] CLT:  %d\n",tick, EcuData.clt);
	}
	if(_ecuLog.ignAngleOn){
			printf("[%d] IGN ANGLE:  %d\n",tick, EcuData.ignAngle);
	}
	if(_ecuLog.lambdaOn){
			printf("[%d] LAMBDA:  %d\n",tick, EcuData.lambda);
	}
	if(_ecuLog.lambdaCorrectionOn){
			printf("[%d]  LAMBDA CORR:  %d\n",tick, EcuData.lambdaCorrection);
	}
	if(_ecuLog.egt1On){
			printf("[%d] EGT1:  %d\n",tick, EcuData.egt1);
	}
	if(_ecuLog.egt2On){
			printf("[%d] EGT2:  %d\n",tick, EcuData.egt2);
	}
	if(_ecuLog.gearOn){
			printf("[%d] GEAR:  %d\n",tick, EcuData.gear);
	}
	if(_ecuLog.ecuTempOn){
			printf("[%d] ECU TEMP:  %d\n",tick, EcuData.ecuTemp);
	}
	if(_ecuLog.errflagOn){
			printf("[%d] ERROR FALGS:  %d\n",tick, EcuData.errflag);
	}
	if(_ecuLog.flags1On){
			printf("[%d] FLAGS1:  %d\n",tick, EcuData.flags1);
	}
	if(_ecuLog.DBWPositionOn){
			printf("[%d] DBW POS:  %d\n",tick, EcuData.DBWPosition);
	}
	if(_ecuLog.DBWTriggerOn){
			printf("[%d] DBW TRIG:  %d\n",tick, EcuData.DBWTrigger);
	}
	if(_ecuLog.TCDRPMRawOn){
			printf("[%d] TCDRPM RAW:  %d\n",tick, EcuData.TCDRPMRaw);
	}
	if(_ecuLog.TCDRPMOn){
			printf("[%d] TCDRPM:  %d\n",tick, EcuData.TCDRPM);
	}
	if(_ecuLog.TCTorqueReductionOn){
			printf("[%d] TC TORQUE:  %d\n",tick, EcuData.TCTorqueReduction);
	}
	if(_ecuLog.PitLimitTorqueReductionOn){
			printf("[%d] PIT LIMIT TORQUE:  %d\n",tick, EcuData.PitLimitTorqueReduction);
	}
	if(_ecuLog.BurnedFuelOn){
			printf("[%d] BURNED FUEL:  %f\n",tick, EcuData.BurnedFuel);
	}

}

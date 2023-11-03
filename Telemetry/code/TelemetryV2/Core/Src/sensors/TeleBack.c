/*
 * TeleBack.c
 *
 *  Created on: 10 lis 2022
 *      Author: Patryk
 */
#include "sensors/TeleBack.h"
extern RTC_HandleTypeDef hrtc;

int TeleBackAnyDataReady(TeleBackData tele){
	return tele.ABSLRReady || tele.ABSRRReady || tele.DamperLRReady || tele.DamperRRReady;
}

void TeleBackInit(TeleBackData * tele){

	tele->File = (FIL*)malloc(sizeof(FIL));

	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(tele->path,"Tele%02d%02d.csv",date.Date,date.Month);
	tele->ABSLRReady = 0;
	tele->ABSRRReady = 0;
	tele->DamperLRReady = 0;
	tele->DamperRRReady = 0;

}

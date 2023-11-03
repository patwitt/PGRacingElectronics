/*
 * ecumaster.c
 *
 *  Created on: Aug 13, 2022
 *      Author: Patryk
 */

#include "ecumaster.h"

void EcuInit(EcumasterData* ecu)
{
	ecu->rpm = 0;
	ecu->tps = 0;
	ecu->iat = 0;
	ecu->map = 0;
	ecu->injPW = 0;
	ecu->baro = 0;
	ecu->batt = 0;
	ecu->clt = 0;
	ecu->ecuTemp = 0;
	ecu->speed = 0;
	ecu->oilTemp = 0;
	ecu->oilPress = 0;
	ecu->ignAngle = 0;
	ecu->ignDwell = 0;
	ecu->lambda = 0;
	ecu->lambdaCorrection = 0;
	ecu->egt1 = 0;
	ecu->egt2 = 0;
	ecu->gear = 0;
	ecu->ecuTemp = 0;
	ecu->batt = 0;
	ecu->errflag = 0;
	ecu->flags1 = 0;
	ecu->DBWPosition = 0;
	ecu->DBWTrigger = 0;
	ecu->TCDRPMRaw = 0;
	ecu->TCDRPM = 0;
	ecu->TCTorqueReduction = 0;
	ecu->PitLimitTorqueReduction = 0;
	ecu->BurnedFuel = 0;
}

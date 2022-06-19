/*
 * GearSensor.h
 *
 *  Created on: 22.05.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_GEARSENSOR_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_GEARSENSOR_H_

#include "Types.h"

typedef enum
{
	GEAR_INIT = 0U,
	GEAR_1 = 1U,
	GEAR_N = 2U,
	GEAR_2 = 3U,
	GEAR_3 = 4U,
	GEAR_4 = 5U,
	GEAR_5 = 6U,
	GEAR_6 = 7U,
	GEAR_IN_PROG = 8U,
	GEAR_INVALID = 9U
}GearStates;

#define N_SAMPLES (4U)
#define SAMPLES_3 (3U)
#define SAMPLES_2 (2U)
#define CHECK_IN_PROG (3U)
#define OVERCURRENT_THRESHOLD (3100U)
#define GEARSENSOR_UNDEFINED (0U)

void GearSensor_Process(void);
void GearSensor_DMAProcess(void);
GearStates GearSensor_GetStateBySensorAdc(uint8 nSamples);
void GearSensor_Init(__IO uint16* const);
GearStates GearSensor_GetState(void);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARSENSOR_H_ */

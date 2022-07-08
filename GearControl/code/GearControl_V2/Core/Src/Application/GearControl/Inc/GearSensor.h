/*
 * GearSensor.h
 *
 *  Created on: 22.05.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_GEARSENSOR_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_GEARSENSOR_H_

#include "Types.h"

typedef enum {
	GEAR_SENS_1 = 0U,
	GEAR_SENS_N = 1U,
	GEAR_SENS_2 = 2U,
	GEAR_SENS_3 = 3U,
	GEAR_SENS_4 = 4U,
	GEAR_SENS_5 = 5U,
	GEAR_SENS_6 = 6U,
	GEAR_SENS_IN_PROG = 7U,
	GEAR_SENS_INVALID = 8U,
	GEAR_SENS_COUNT = 9U
} GearSensorStatesEnum;

ErrorEnum GearSensor_Init(void);
void GearSensor_Process(void);
GearSensorStatesEnum GearSensor_GetState(void);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARSENSOR_H_ */

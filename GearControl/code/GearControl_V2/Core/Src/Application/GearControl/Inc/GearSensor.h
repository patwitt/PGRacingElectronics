/*
 * GearSensor.h
 *
 *  Created on: 06.07.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_GEARSENSOR_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_GEARSENSOR_H_

#include "Types.h"

typedef enum {
	GEAR_SENS_PLAUSIBLE,
	GEAR_SENS_IMPLAUSIBLE,
	GEAR_SENS_PLAUSIBILITY_IN_PROGRESS,
	GEAR_SENS_PLAUSIBILITY_CNT
} GearSensorPlausibilityEnum;

typedef enum {
	/* ! This order must be unchanged ! begin */
	GEAR_SENS_1           = 0U,
	GEAR_SENS_N           = 1U,
	GEAR_SENS_2           = 2U,
	GEAR_SENS_3           = 3U,
	GEAR_SENS_4           = 4U,
	GEAR_SENS_5           = 5U,
	GEAR_SENS_6           = 6U,
	GEAR_SENS_IN_PROG     = 7U,
	GEAR_SENS_INVALID     = 8U,
	/* ! This order must be unchanged ! end */
	GEAR_SENS_UNKNOWN     = 9U,
	GEAR_SENS_IMPLAUS     = 10U,
	GEAR_SENS_COUNT       = 11U
} GearSensorStatesEnum;

ErrorEnum GearSensor_Init(void);
void GearSensor_Process(void);
GearSensorStatesEnum GearSensor_GetState(void);
GearSensorPlausibilityEnum GearSensor_GetPlausibility(void);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARSENSOR_H_ */

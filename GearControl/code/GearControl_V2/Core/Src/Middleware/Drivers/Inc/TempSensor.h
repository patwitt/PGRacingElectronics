/*
 * TempSensor.h
 *
 *  Created on: 13.04.2024
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_MIDDLEWARE_DRIVERS_INC_TEMP_SENSOR_H_
#define SRC_MIDDLEWARE_DRIVERS_INC_TEMP_SENSOR_H_

#include "Types.h"

typedef enum {
	SENS_OK,
	SENS_RECV_FAIL,
	SENS_NO_RESPONSE
} TempSensorStatus;

TempSensorStatus TempSensor_GetStatus(void);
void TempSensor_Task(void);

#endif /* SRC_MIDDLEWARE_DRIVERS_INC_TEMP_SENSOR_H_ */

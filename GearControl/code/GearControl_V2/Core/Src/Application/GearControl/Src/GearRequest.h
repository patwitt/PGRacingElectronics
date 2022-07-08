/*
 * GearRequest.h
 *
 *  Created on: 22.05.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_GEARREQUEST_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_GEARREQUEST_H_

#include "Types.h"

typedef enum {
	GEAR_REQUEST_NONE,
	GEAR_REQUEST_SHIFT_DOWN,
	GEAR_REQUEST_SHIFT_N,
	GEAR_REQUEST_SHIFT_UP,
	GEAR_REQUEST_INVALID
} GearRequestEnum;

ErrorEnum GearRequest_Init(void);
GearRequestEnum GearRequest_Get(void);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARREQUEST_H_ */

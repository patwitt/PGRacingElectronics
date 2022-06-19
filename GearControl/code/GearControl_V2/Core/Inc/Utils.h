/*
 * GearSensor.h
 *
 *  Created on: 22.05.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "Types.h"

static __IO inline void Utils_UpdateMax_U16(const uint16 value, uint16 *const max)
{
	if (value > *max) {
		*max = value;
	}
}

static __IO inline void Utils_UpdateMinMax_F(const float value, float *const min, float *const max)
{
	if (value < *min) {
		*min = value;
	}
	if (value > *max) {
		*max = value;
	}
}

static __IO inline void Utils_UpdateMinMax_U16(const uint16 value, uint16 *const min, uint16 *const max)
{
	if (value < *min) {
		*min = value;
	}
	if (value > *max) {
		*max = value;
	}
}

#endif /* INC_UTILS_H_ */

/*
 * GearControl.h
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_DBW_H_
#define INC_DBW_H_

#if 0
	static uint32 ticks = 0;

	if (ticks < 300) {
		if (*etc.PWM_H_minus == 0) {
			*etc.PWM_H_plus = 1500;
		}
		*etc.PWM_H_minus = 0;
	} else {
		if (*etc.PWM_H_plus == 0) {
			*etc.PWM_H_minus = 1500;
		}
		*etc.PWM_H_plus = 0;
	}

	++ticks;
	if (ticks > 600) {
		ticks = 0U;
	}
#endif

#include "Types.h"

void DBW_Process(void);
ErrorEnum DBW_Init(void);
void DBW_Disable(boolean isError);
void DBW_RequestAppsCalibration(void);
#endif /* INC_DBW_H_ */

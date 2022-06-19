/*
 * PID.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_PID_AUTOTUNE_H_
#define INC_PID_AUTOTUNE_H_

/*********** Macros ***********/
#if 0
#include "main.h"

typedef struct
{
	/* Gains */
	float Kp;
	float Ki;
	float Kd;

	/* LPF constant */
	float tau;

	/* Limits */
	float limMin;
	float limMax;

	/* Samping time */
	float T;

	/* previous samples */
	float integrator;
	float prevErr; //! For integrator
	float differentiator;
	float prevMeas; //! For differentiator

	/* Output */
	float out;
} PIDController;

void PID_Init(PIDController* pid);
float PID_Update(float setPoint, float measurement);
#endif
#endif /* INC_PID_AUTOTUNE_H_ */

/*
 * PID.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_PID_H_
#define INC_PID_H_

/*********** Macros ***********/

#include "Types.h"

typedef struct
{
	/* Gains */
	float Kp;
	float Ki;
	float Kd;

	/* LPF constant */
	float tau;

	/* Limits */
	const float limMin;
	const float limMax;

	/* Samping time */
	const float T;

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

#endif /* INC_PID_H_ */

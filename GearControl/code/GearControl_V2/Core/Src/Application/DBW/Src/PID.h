/*
 * PID.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_SRC_PID_H_
#define SRC_APPLICATION_DBW_SRC_PID_H_

/*********** Macros ***********/

#include "Types.h"

typedef struct
{
	/* Gains */
	float Kp;
	float Ki;
	float Kd;

	/* LPF constant */
	const float tau;

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

float PID_Update(const float target, const float measurement);

#endif /* SRC_APPLICATION_DBW_SRC_PID_H_ */

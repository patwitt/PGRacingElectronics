/*
 * PID.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_SRC_PID_H_
#define SRC_APPLICATION_DBW_SRC_PID_H_

#include "Types.h"
#include "Utils.h"

#define MEASURE_SLOPE_MS (5U)

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
	float error;
	float prevErr; //! For integrator
	float differentiator;
	float prevMeas; //! For differentiator
	float prevTarget;

	float deltaMeas;
	float deltaTarget;
	float deltaErr;
	Slope slope;
	AvgBuffer_F avgSlopeData;
	/* Output */
	float out;
} PIDController;

#if 0
	/* Derivative band-limited differentiator */
	pid.differentiator = -(2.0f * pid.Kd * (measurement - pid.prevMeas)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
	                        + (2.0f * pid.tau - pid.T) * pid.differentiator)
	                        / (2.0f * pid.tau + pid.T);
#endif

float PID_Update(float *target, const float measurement);

#endif /* SRC_APPLICATION_DBW_SRC_PID_H_ */

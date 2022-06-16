/*
 * PID.c
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "PID.h"

static PIDController pid = {0.0f};

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

void PID_Init(PIDController* pid)
{
	(void)pid;
}

float PID_Update(float setPoint, float measurement)
{
	/* Error signal */
	float error = setPoint - measurement;

	/* Proportional */
	float proportional = pid.Kp * error;

	/* Integral */
	pid.integrator = pid.integrator + 0.5f * pid.Ki * pid.T * (error + pid.prevErr);

	/* Anti wind-up via dynamic integrator clamping */
	float limMinInt;
	float limMaxInt;

	if (pid.limMax > proportional)
	{
		limMaxInt = pid.limMax - proportional;
	}
	else
	{
		limMaxInt = 0.0f;
	}

	if (pid.limMin < proportional)
	{
		limMinInt = pid.limMin - proportional;
	}
	else
	{
		limMinInt = 0.0f;
	}

	/* Integrator clamp */
	if (pid.integrator > limMaxInt)
	{
		pid.integrator = limMaxInt;
	}
	if (pid.integrator < limMinInt)
	{
		pid.integrator = limMinInt;
	}

	/* Derivative band-limited differentiator */
	pid.differentiator = (2.0f * pid.Kd * (measurement - pid.prevMeas)
			           + (2.0f * pid.tau - pid.T) * pid.differentiator)
					   / (2.0f * pid.tau + pid.T);

	/* Compute output */
	pid.out = proportional + pid.integrator + pid.differentiator;

	if (pid.out > pid.limMax)
	{
		pid.out = pid.limMax;
	}
	if (pid.out < pid.limMin)
	{
		pid.out = pid.limMin;
	}

	pid.prevErr = error;
	pid.prevMeas = measurement;

	return pid.out;
}

/*
 * PID.c
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "PID.h"
#include "DefineConfig.h"

static PIDController pid = {.limMax = 1000.0f, .limMin = 0.0f, .T = 0.1f};

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

void PID_Init(PIDController* pid)
{
	(void)pid;
}

float PID_Update(float target, float measurement)
{
	/* Error signal */
	float error = target - measurement;

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
	pid.integrator = CLAMP_MAX(pid.integrator, limMaxInt);
	pid.integrator = CLAMP_MIN(pid.integrator, limMinInt);

	/* Derivative band-limited differentiator */
	pid.differentiator = (2.0f * pid.Kd * (measurement - pid.prevMeas)
			           + (2.0f * pid.tau - pid.T) * pid.differentiator)
					   / (2.0f * pid.tau + pid.T);

	/* Compute output */
	pid.out = proportional + pid.integrator + pid.differentiator;

	/* Clamp output */
	pid.out = CLAMP_MAX(pid.out, pid.limMax);
	pid.out = CLAMP_MIN(pid.out, pid.limMin);

	/* Store error */
	pid.prevErr = error;
	pid.prevMeas = measurement;

	return pid.out;
}

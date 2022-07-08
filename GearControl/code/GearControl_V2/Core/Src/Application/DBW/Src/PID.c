/*
 * PID.c
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "PID.h"
#include "PIDHacks.h"
#include "PID_InterpolationTables.h"
#include "Utils.h"
#include "DBW.h"
#include "DefineConfig.h"

/* ---------------------------- */
/*         Local data           */
/* ---------------------------- */

#define PID_OUTPUT_SPRING_BIAS (120.0f)

#if CONFIG_PID_INTERPOLATE_CONSTANTS
static const PID_InterpolationTableEnum currentTable = PID_INTEPROLATE_WORKING_TABLE;
static PIDController pid = {.limMax = 1000.0f, .limMin = -1000.0f, .tau = 1.0f, .T = 0.001f, .Kp = 0.0f, .Ki = 0.0f, .Kd = 0.0f, .differentiator = 0.0f};
#else
static PIDController pid = {.limMax = 1000.0f, .limMin = -1000.0f, .tau = 1.0f, .T = 0.001f, .Kp = 6.66f, .Ki = 11.89f, .Kd = 15.69f, .differentiator = 0.0f, .avgSlopeData.nSamples = N_SAMPLES_F};
#endif

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
float PID_Update(float *target, const float measurement)
{
	static uint32_t sampleCnt = 0U;

#if CONFIG_PID_APPLY_BRAKE
	PIDHacks_UpdateSlope(&pid, &sampleCnt, measurement);
	PIDHacks_SlowDownSequence(&pid, target, measurement);
#endif

	/* Error signal */
	pid.error = *target - measurement;

#if CONFIG_PID_INTERPOLATE_CONSTANTS
	/* Interpolate Kp, Ki, Kd gains as per look-up tables */
	const float errorAbs = pid.error > 0.0f ? pid.error : (pid.error * (-1.0f));
	pid.Kp = Utils_interpolateTable1d(PID_InterpolationTables[currentTable].table1d_Kp, errorAbs);
	pid.Ki = Utils_interpolateTable1d(PID_InterpolationTables[currentTable].table1d_Ki, errorAbs);
	pid.Kd = Utils_interpolateTable1d(PID_InterpolationTables[currentTable].table1d_Kd, errorAbs);
#endif

	/* Proportional */
	const float proportional = pid.Kp * pid.error;

	/* Integral */
	pid.integrator = pid.integrator + 0.5f * pid.Ki * pid.T * (pid.error + pid.prevErr);

	/* Anti wind-up via dynamic integrator clamping */
	float limMinInt = 0.0f;
	float limMaxInt = 0.0f;

	if (pid.limMax > proportional) {
		limMaxInt = pid.limMax - proportional;
	}

	if (pid.limMin < proportional) {
		limMinInt = pid.limMin - proportional;
	}

	/* Integrator clamp */
	pid.integrator = CLAMP_MAX(pid.integrator, limMaxInt);
	pid.integrator = CLAMP_MIN(pid.integrator, limMinInt);

	pid.deltaMeas = measurement - pid.prevMeas;
	pid.differentiator = pid.Kd * pid.deltaMeas * pid.T;

	/* Compute output */
	pid.out = proportional + pid.integrator + pid.differentiator;

	/* Spring BIAS */
	if (pid.out > 0.0f) {
		pid.out += PID_OUTPUT_SPRING_BIAS;
	}

#if CONFIG_PID_APPLY_BRAKE
	PIDHacks_ApplyBrake(&pid, target);
#endif

	/* Clamp output */
	pid.out = CLAMP_MAX(pid.out, pid.limMax);
	pid.out = CLAMP_MIN(pid.out, pid.limMin);

	/* Store */
	pid.prevErr = pid.error;
	pid.prevMeas = measurement;

	++sampleCnt;
	sampleCnt %= (MEASURE_SLOPE_MS + 1U);

	return pid.out;
}


/*
 * PIDHacks.h
 *
 *  Created on: 03.07.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_SRC_PIDHACKS_H_
#define SRC_APPLICATION_DBW_SRC_PIDHACKS_H_

#include "Types.h"
#include "PID.h"
#include "Utils.h"
#include "DBW.h"

/* ---------------------------- */
/*         Local data           */
/* ---------------------------- */
#define MAX_SLOWDOWN_APPLIES (12U)
#define SLOPE_DIV (5.0f)

#define TARGET_POS_RESET_SLOWDOWN (155.0f)
#define TARGET_ADD_OFFSET_MAX (73.0f)
#define TARGET_SLOPE_MULTIPLIER (5.0f)
#define SLOPE_APPLY_SLOWDOWN_MIN (-5.5f)
#define TPS_MIN_POS_TO_APPLY_SLOWDOWN (200.0f)

#define SLOPE_MIN_TO_APPLY_BRAKE_GOING_UP (15.0f)
#define TARGET_MAX_TO_APPLY_BRAKE_GOING_UP (900.0f)
#define TARGET_CLAMP_BRAKE_GOING_UP (700.0f)
#define TARGET_MIN_TO_APPLY_BRAKE_GOING_DOWN (420.0f)
#define TARGET_CLAMP_BRAKE_GOING_DOWN (-600.0f)
#define TARGET_CLAMP_GOING_DOWN_CRITICAL_SLOPE (-6.0f)
#define TARGET_CLAMP_CRITICAL_BRAKE_GOING_DOWN (-120.0f)

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

static inline void PIDHacks_ApplyBrake(PIDController *const pid, float *const target)
{
	/* This should brake for too big overshoot */
	if ((pid->slope.val > SLOPE_MIN_TO_APPLY_BRAKE_GOING_UP) && (*target < TARGET_MAX_TO_APPLY_BRAKE_GOING_UP)) {
		//pid->out = CLAMP_MAX(pid->out, TARGET_CLAMP_BRAKE_GOING_UP);
	}

	/* This should brake for too big undershoot */
	if ((pid->out < 0.0f) && (*target < TARGET_MIN_TO_APPLY_BRAKE_GOING_DOWN)) {
		pid->out = CLAMP_MIN(pid->out, TARGET_CLAMP_BRAKE_GOING_DOWN);

		if (pid->slope.val < TARGET_CLAMP_GOING_DOWN_CRITICAL_SLOPE) {
			pid->out = CLAMP_MIN(pid->out, TARGET_CLAMP_CRITICAL_BRAKE_GOING_DOWN);
		}
	}
}

static inline void PIDHacks_UpdateSlope(PIDController *const pid, uint32_t *const sampleCnt, const float measurement)
{
	/* Update slope stats every MEASURE_SLOPE_MS */
	if (*sampleCnt == 0U) {
		pid->slope.y1 = measurement;
	} else if ((*sampleCnt % MEASURE_SLOPE_MS) == 0U) {
		pid->slope.y2 = measurement;
		pid->slope.val = Utils_CalcSlope(&pid->slope, SLOPE_DIV);
		Utils_RollingAverage_F(&pid->avgSlopeData, pid->slope.val);
	} else { /* Do nothing */ }
}

static inline void PIDHacks_SlowDownSequence(PIDController *const pid, float *const target, const float measurement)
{
	static bool_t slowDownInProg = FALSE;
	static uint32_t slowDownCnt = 0U;
	static float slowDownTarget = 0.0f;

	/* Detect slow down criteria for IDLE undershoot */
	if (!slowDownInProg) {
		if ((*target < TPS_IDLE + 0.02f) && (measurement < TPS_MIN_POS_TO_APPLY_SLOWDOWN) && (pid->avgSlopeData.avg < SLOPE_APPLY_SLOWDOWN_MIN)) {
			slowDownTarget = pid->avgSlopeData.avg;
			slowDownInProg = TRUE;
		}
	}

	/* Apply slow down by increasing target to not undershoot below IDLE */
	if (slowDownInProg) {
		if ((pid->avgSlopeData.avg < SLOPE_APPLY_SLOWDOWN_MIN) && (slowDownCnt < MAX_SLOWDOWN_APPLIES)) {
			const float addOffset = CLAMP_MAX((TARGET_SLOPE_MULTIPLIER * ABS_F(slowDownTarget)), TARGET_ADD_OFFSET_MAX);
			*target += addOffset;
		} else {
			slowDownTarget = 0.0f;
			slowDownInProg = FALSE;
		}
		++slowDownCnt;
	}

	if (*target > TARGET_POS_RESET_SLOWDOWN) {
		slowDownCnt = 0U;
	}
}

#endif /* SRC_APPLICATION_DBW_SRC_PIDHACKS_H_ */

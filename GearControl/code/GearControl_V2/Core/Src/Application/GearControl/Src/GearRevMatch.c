/*
 * GearRevMatch.c
 *
 *  Created on: 4 sie 2024
 *
 */
#include "GearRevMatch.h"
#include "GearControl.h"

typedef struct {
	const uint32_t timeMs[GEAR_UNKNOWN];
	const float tpsTarget[GEAR_UNKNOWN];
	SwTimerType timer;
} GearRevMatch;

static GearRevMatch revMatch_ = {
	/* 0 -> REVMATCH DISABLED */
	.timeMs = {
			[GEAR_1] = 0U,
			[GEAR_N] = 0U,
			[GEAR_2] = 0U,
			[GEAR_3] = 300U,
			[GEAR_4] = 200U,
			[GEAR_5] = 200U,
			[GEAR_6] = 200U
	},
	.tpsTarget = {
			[GEAR_1] = 0.0f,
			[GEAR_N] = 0.0f,
			[GEAR_2] = 0.0f,
			[GEAR_3] = 400.0f,
			[GEAR_4] = 150.0f,
			[GEAR_5] = 200.0f,
			[GEAR_6] = 200.0f
	}
};

bool_t GearRevMatch_IsActive(void)
{
	return SwTimerIsActive(&revMatch_.timer);
}

float GearRevMatch_GetTpsTarget(void)
{
	const GearStates gear = GearControl_GetGear();

	return (gear <= GEAR_6) ? revMatch_.tpsTarget[gear] : 0.0f;
}

ErrorEnum GearRevMatch_Init(void)
{
	return SwTimerRegister(&revMatch_.timer);
}

void GearRevMatch_Trigger(const GearStates gear)
{
	if (gear <= GEAR_6 && revMatch_.timeMs[gear] > 0U) {
		SwTimerRestart(&revMatch_.timer, revMatch_.timeMs[gear]);
	}
}

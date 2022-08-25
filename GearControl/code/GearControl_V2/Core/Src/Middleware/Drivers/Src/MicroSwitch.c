/*
 * MicroSwitch.c
 *
 *  Created on: 24.04.2021
 *      Author: Patryk Wittbrodt
 */

#include "MicroSwitch.h"
#include "main.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
#define DEBOUNCE_10MS   (50U)
#define DEBOUNCE_20MS   (50U)

static MicroSwitchInternal microSwitchUp = {.debCnt = 0U, .validCnt = 0U, .GPIO = GPIO_PIN_1};
static MicroSwitchInternal microSwitchDown = {.debCnt = 0U, .validCnt = 0U, .GPIO = GPIO_PIN_2};

static __IO MicroSwitch microSwitches[MS_COUNT] = {
	{.internal = &microSwitchDown},
	{.internal = &microSwitchUp}
};

static MicroSwitchControlType microSwitchControl = MS_CONTROL_DEBOUNCE_LOW;

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static void MicroSwitch_StateMachine(void);
static void MicroSwitch_DebounceLow(void);
static void MicroSwitch_NormalOperation(void);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */

/**
 * @brief MicroSwitch debouncing for LOW states.
 * 
 * If both microswitches are in LOW state for at least 20ms to low,
 * go to ENABLED state.
 */
static void MicroSwitch_DebounceLow(void)
{
	static uint32_t lowDebCnt = 0U;

	/* Make sure both microswitches were LOW debouncing before triggering gear shifting again */
	if ((HAL_GPIO_ReadPin(GPIOC, microSwitches[MS_G_UP].internal->GPIO) == GPIO_PIN_RESET) &&
		(HAL_GPIO_ReadPin(GPIOC, microSwitches[MS_G_DOWN].internal->GPIO) == GPIO_PIN_RESET)) {
		++lowDebCnt;

		if (lowDebCnt > DEBOUNCE_20MS)
		{
			lowDebCnt = 0U;
			microSwitchControl = MS_CONTROL_ENABLED;
		}
	} else {
		lowDebCnt = 0U;
	}
}

/**
 * @brief MicroSwitches normal operation.
 * 
 * Debounces microswitches for 10ms and if any is detected to be HIGH,
 * changes state of that microswitch to HIGH. Otherwise it's set to LOW.
 */
static void MicroSwitch_NormalOperation(void)
{
	for (uint32_t i = 0U; i < MS_COUNT; ++i) {
		__IO MicroSwitch *const microswitch = &microSwitches[i];

		if (microswitch->internal->debCnt <= DEBOUNCE_10MS) {
			microswitch->state = MS_STATE_DEBOUNCING;

			++microswitch->internal->debCnt;
			if (HAL_GPIO_ReadPin(GPIOC, microswitch->internal->GPIO) == GPIO_PIN_SET) {
				++microswitch->internal->validCnt;
			} else {
				microswitch->internal->validCnt = 0U;
			}
		} else {
			microswitch->state = (microswitch->internal->validCnt >= DEBOUNCE_10MS) ? MS_STATE_HIGH : MS_STATE_LOW;
			microswitch->internal->debCnt = 0U;
			microswitch->internal->validCnt = 0U;
		}
	}
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

/**
 * @brief Initialization of the MicroSwitch module.
 * 
 * It resets all internal variables.
 */
void MicroSwitch_Init(void)
{
	for (uint32_t i = 0U; i < MS_COUNT; ++i) {
		microSwitches[i].internal->debCnt = 0U;
		microSwitches[i].internal->validCnt = 0U;
		microSwitches[i].state = MS_STATE_DEBOUNCING;
	}
}

/**
 * @brief MicroSwitch module main state machine.
 */
static void MicroSwitch_StateMachine(void)
{
	switch (microSwitchControl) {
		case MS_CONTROL_DEBOUNCE_LOW:
			MicroSwitch_DebounceLow();
			break;

		case MS_CONTROL_ENABLED:
			MicroSwitch_NormalOperation();
			break;

		case MS_CONTROL_DISABLED:
			microSwitches[MS_G_UP].state = MS_STATE_DEBOUNCING;
			microSwitches[MS_G_DOWN].state = MS_STATE_DEBOUNCING;

		default:
			break;
	}
}

/**
 * @brief Main process function that is called from the Scheduler.
 */
void MicroSwitch_Process(void)
{
	MicroSwitch_StateMachine();
}

/**
 * @brief Get microswitch entity.
 * 
 *  This function returns a pointer to a MicroSwitch struct.
 * 
 * @param microswitch The microswitch to get.
 * 
 * @return A pointer to a MicroSwitch struct.
 */
__IO MicroSwitch* MicroSwitch_Get(MicroSwitchTypeEnum microswitch)
{
	__IO MicroSwitch* mSwitch = NULL;

	if (microswitch < MS_COUNT) {
		mSwitch = &microSwitches[microswitch];
	}

	return mSwitch;
}

/**
 * @brief Set Control of Microswitches.
 * 
 * Used by upper layers for changing control of MicroSwitches.
 * Mainly used for changing to LOW debouncing.
 * 
 * @param control The control type to set.
 */
void MicroSwitch_SetControl(MicroSwitchControlType control)
{
	microSwitchControl = control;
}

/**
 * This function returns the current control state of the micro switch.
 * 
 * @return The current microswitch control.
 */
MicroSwitchControlType MicroSwitch_GetControl(void)
{
	return microSwitchControl;
}

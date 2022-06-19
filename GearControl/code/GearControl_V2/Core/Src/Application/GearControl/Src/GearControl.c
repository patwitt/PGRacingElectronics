/*
 * GearControl.c
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */
#include <CAN.h>
#include "GearControl.h"

#include "GearRequest.h"
#include "GearWatchdog.h"
#include "MicroSwitch.h"
#include "GearSensor.h"
#include "Servo.h"
#if 0
extern TIM_HandleTypeDef htim3;

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

typedef struct
{
	__IO ShiftStates state;
	__IO uint8 request;
	__IO uint8 validation;
}ShiftCtrl;

typedef struct
{
	GearStates state;
	uint8 degreesUp;
	uint8 degreesDown;
}GearData;

#define DEG_1_TO_N (32U)
#define DEG_UP (80U)
#define DEG_DOWN (DEG_UP + 5U)
#define DEG_N_DOWN (70U)
#define DEG_N_UP (86U)
#define DEG_1_2 (90U)

static const GearData GearDataDecoder[] = {
		{GEAR_INIT, 0U,        0U},
		{GEAR_1,    DEG_1_2,   0U},
		{GEAR_N,    DEG_N_DOWN,DEG_N_UP},
		{GEAR_2,    DEG_UP,    DEG_1_2},
		{GEAR_3,    DEG_UP,    DEG_DOWN},
		{GEAR_4,    DEG_UP,    DEG_DOWN},
		{GEAR_5,    DEG_UP,    DEG_DOWN},
		{GEAR_6,    0U,        DEG_DOWN}
};

typedef struct
{
	__IO uint32 PwmRequest;
	__IO uint32 errors;
	__IO uint8 wdgCnt;
	__IO GearStates prevGear;
	__IO GearStates state;
	__IO GearStates nextState;
	__IO GearSensorStates GearSensState;
	__IO boolean wdgStatus;
	__IO boolean activation;
	__IO boolean cutStatus;
	const GearData* data;
}GearCtrl;

static __IO ShiftCtrl ShiftHandler;
static __IO GearCtrl GearHandler;
static __IO uint32 ticks = 0;

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */

static GearStates GearState_Init(void);
static GearStates GearStateHandler(void);

static void GearControl_RequestGearChange(__IO uint8*, GearStates*);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */

ErrorEnum GearControlInit(void) {
	ErrorEnum err = ERROR_OK;

	err = GearRequestsInit();

	if (err == ERROR_OK) {
		err = GearWatchdogInit();
	}

	if (err == ERROR_OK) {
		if (htim3.Instance != NULL) {
			err = ServoInit(SERVO_GEAR_SHIFT, &htim3.Instance->CCR3);
		} else {
			err = ERROR_NULL;
		}
	}

	return err;
}


/* Gear state init handler */
static GearStates GearState_Init(void)
{
	nextState = GEAR_N;

	return nextState;
}

static void GearControl_RequestGearChange(__IO uint8* request, GearStates* savedState)
{
	/* Valid request, change gear */
	switch (*request)
	{
		case REQUEST_SHIFT_DOWN:
			//GearControl_SetServoPos(SET_DEG, (POS_DEG_DEFAULT - GearHandler.data->degreesDown), 0);
			if (GEAR_2 == GearHandler.state)
			{
				/* On normal request from 2 skip N to 1 gear */
				*savedState = (GearStates)((uint8)GearHandler.state - 2U);
			}
			else
			{
				*savedState = (GearStates)((uint8)GearHandler.state - 1U);
			}
			break;

		case REQUEST_SHIFT_UP:
			//GearControl_SetServoPos(SET_DEG, (POS_DEG_DEFAULT + GearHandler.data->degreesUp), 0);
			if (GEAR_1 == GearHandler.state)
			{
				/* On normal request from 1 skip N to 2 gear */
				*savedState = (GearStates)((uint8)GearHandler.state + 2U);
			}
			else
			{
				*savedState = (GearStates)((uint8)GearHandler.state + 1U);
			}
			break;

		case REQUEST_SHIFT_N:
			//GearControl_SetServoPos(SET_DEG, (POS_DEG_DEFAULT + DEG_1_TO_N), 0);
			*savedState = (GearStates)((uint8)GearHandler.state + 1U);
			break;

		default:
			break;
	}
}

/* Gear state handler, called every X ms */
static GearStates GearStateHandler(void)
{
	static GearStates savedState = GEAR_INVALID;
	static uint8 fuelCutDelay = 0U;
	GearStates nextState = GearHandler.state;

	switch (ShiftHandler.state)
	{
		case SHIFT_INIT:
			ShiftHandler.state = SHIFT_IDLE;
			break;

		case SHIFT_IDLE:
			if (ShiftHandler.request != REQUEST_EMPTY) {
				if (NULL != GearHandler.data) {
					ShiftHandler.state = SHIFT_PRELOAD;

					/* Save previous gear */
					GearHandler.prevGear = GearHandler.state;

					/* Set servo to change position */
					//GearControl_RequestGearChange(&ShiftHandler.request, &savedState);

					/* Reset request and error, start WDG Timer */
					ShiftHandler.request = REQUEST_EMPTY;
					ShiftHandler.validation = VALIDATION_POLL;
					GearHandler.wdgStatus = TRUE;
				}
			}
			break;

		case SHIFT_PRELOAD:
			/* Wait for fuel cut */
			/* GEAR_CUT_DELAY * 4ms */
			if ((++fuelCutDelay) >= GEAR_CUT_DELAY)
			{
				fuelCutDelay = 0U;
				/* Trigger fuel cut */
				GearControl_FuelCutTrigger(ON);

				GearHandler.nextState = savedState;
				ShiftHandler.state = SHIFT_EXEC;
			}
			break;

		case SHIFT_DONE:
			GearControl_FuelCutTrigger(OFF);
			GearHandler.nextState = GEAR_INVALID;
			GearHandler.data = NULL;
			nextState = savedState;
			//GearControl_WdgOff();
			ShiftControl_SetState(SHIFT_IDLE);
			break;

		case SHIFT_EXEC:
		default:
			/* In progress */
			break;
	}

	return nextState;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
void ShiftControl_SetRequest(uint8 request)
{
	if (TRUE == GearControl_IsActive())
	{
		uint8 req = GearState_ValidateRequest(request);

		if (REQUEST_EMPTY != req)
		{
			/* Valid Request */
			ShiftHandler.request = req;
			GearHandler.data = &GearDataDecoder[(uint8)GearHandler.state];
		}
		else
		{
			GearControl_SetError(SHIFT_REQUEST_ERROR, TRUE);
		}
	}
	else
	{
		/* Gear Change disabled */
		GearControl_FuelCutTrigger(OFF);
		//GearControl_SetServoPos(SET_DEG, POS_DEG_DEFAULT, 0);
		MicroSwitch_PollAgain();
	}
}

/* Gear control main process */
void GearControl_Process(void)
{
	switch (GearHandler.state) {
		case GEAR_INIT:
			GearHandler.state = GearState_Init();
			break;

		default:
			GearHandler.state = GearStateHandler();
			break;
	}

	GearWatchdogProcess();
}

void GearControl_FuelCutTrigger(uint8 onOff)
{
	if (onOff == ON)
	{
		GearHandler.cutStatus = TRUE;
		HAL_GPIO_WritePin(GEAR_CUT_GPIO_Port, GEAR_CUT_Pin, GPIO_PIN_SET);
	}
	else
	{
		GearHandler.cutStatus = FALSE;
		HAL_GPIO_WritePin(GEAR_CUT_GPIO_Port, GEAR_CUT_Pin, GPIO_PIN_RESET);
	}
}
#endif

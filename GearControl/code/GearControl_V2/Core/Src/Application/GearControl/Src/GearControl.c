/*
 * GearControl.c
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */
#include "CAN.h"
#include "GearControl.h"

#include "GearRequest.h"
#include "GearWatchdog.h"
#include "MicroSwitch.h"
#include "GearSensor.h"
#include "Servo.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

typedef enum {
	GEAR_1     = 0U,
	GEAR_N     = 1U,
	GEAR_2     = 2U,
	GEAR_3     = 3U,
	GEAR_4     = 4U,
	GEAR_5     = 5U,
	GEAR_6     = 6U,
	GEAR_INIT  = 7U
} GearCtrlStates;

typedef struct {
	GearCtrlStates state;
} GearControlHandler;

static __IO GearControlHandler gearCtrl = {.state = GEAR_INIT};

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */
static GearCtrlStates GearCtrlState_Init(void);
static GearCtrlStates GearCtrlState_Handler(void);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
/* Gear state Initialization handler */
static GearCtrlStates GearCtrlState_Init(void)
{
	GearCtrlStates nextState = GEAR_INIT;
	GearSensorStatesEnum gearSensState = GearSensor_GetState();

	if (gearSensState <= GEAR_SENS_6) {
		nextState = (GearCtrlStates)gearSensState;
	}

	return nextState;
}

/* Gear state handler, called every 1 ms */
static GearCtrlStates GearCtrlState_Handler(void)
{
	GearCtrlStates nextState = gearCtrl.state;

	/*
	 * @TODO: Implementation of gear handlers.
	 */

	return nextState;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum GearControl_Init(TIM_HandleTypeDef *htim)
{
	ErrorEnum err = ERROR_OK;

	/* Initialize Gear Requests module */
	if (err == ERROR_OK) {
		err = GearRequest_Init();
	}

	/* Initialize Gear Watchdog module */
	if (err == ERROR_OK) {
		err = GearWatchdog_Init();
	}

	/* Initialize Servo module */
	if (err == ERROR_OK) {
		if ((htim != NULL) && (htim->Instance != NULL)) {
			err = Servo_Init(SERVO_GEAR_SHIFT, &htim->Instance->CCR3);
		} else {
			err = ERROR_NULL;
		}
	}

	return err;
}

/* Gear control main process */
void GearControl_Process(void)
{
	switch (gearCtrl.state) {
		case GEAR_INIT:
			gearCtrl.state = GearCtrlState_Init();
			break;

		default:
			gearCtrl.state = GearCtrlState_Handler();
			break;
	}

	GearWatchdog_Process();
}


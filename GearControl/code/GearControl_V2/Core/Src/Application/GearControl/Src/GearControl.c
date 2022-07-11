/*
 * GearControl.c
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */
#include "CAN.h"
#include "GearControl.h"

#include "Utils.h"
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

static const ServoConfig gearServoConfig = {
		.limits = {
			 .degMin     = 20U, //!< 70 deg down
		     .degDefault = 90U,
			 .degMax     = 160U //!< 70 deg up
		},
		.pwmChannel = TIM_CHANNEL_3
};

typedef struct {
	GearCtrlStates state;
	const ServoTypeEnum servo;
} GearControlHandler;

static __IO GearControlHandler gearCtrl = {.state = GEAR_INIT, .servo = SERVO_GEAR_SHIFT};

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
ErrorEnum GearControl_Init(TIM_HandleTypeDef *const htim)
{
	ErrorEnum err = ERROR_OK;

	/* Initialize Gear Requests module */
	err = GearRequest_Init();

	/* Initialize Gear Watchdog module */
	if (err == ERROR_OK) {
		err = GearWatchdog_Init();
	}

	/* Initialize gear servo */
	if (err == ERROR_OK) {
		NULL_ERR_CHECK2(err, htim, htim->Instance);

		if (err == ERROR_OK) {
			/* Gear Control servo PWM parameters */
			ServoPwmParams gearServoPwmParams = {
				.htim = htim,
				.PWM = &htim->Instance->CCR3
			};

			err = Servo_Init(gearCtrl.servo, &gearServoConfig, gearServoPwmParams);
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


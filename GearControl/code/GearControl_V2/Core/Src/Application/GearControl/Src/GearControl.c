/*
 * GearControl.c
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */

#include "GearControl.h"
#include "ShiftControl.h"

#include "Utils.h"
#include "CAN.h"
#include "GearControlCAN.h"
#include "GearWatchdog.h"
#include "GearRequest.h"
#include "Servo.h"
#include "MicroSwitch.h"
#include "GearControlMap.h"
#include "GearSensor.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

#define SHIFT_TIME_STATS_MAX (0xFFU)

//! Gear Control main handler struct
typedef struct {
	const ServoConfig *const servoConfig;           //!< Gear servo configuration
	GearStates gear;                                //!< Actual gear by estimation and sensor reading
	const ServoEntityEnum servo;                    //!< Servo Entity Type
	const CANShiftStatus *const canShiftStatusMap;  //!< CAN Bus shift mapping - CAN signal <- shift status
	SwTimerStats timStats;                          //!< Gear shifting statistics
} GearControlHandler;

//! Mapping of shift state <- CAN shift status signal
static const CANShiftStatus shiftCanMap[SHIFT_STATES_COUNT + 1U] = {
	[SHIFT_INIT]                 = CAN_SHIFT_INIT,
	[SHIFT_IDLE]                 = CAN_SHIFT_IDLE,
	[SHIFT_PROCEDURE_UP]         = CAN_SHIFT_EXEC,
	[SHIFT_PROCEDURE_DOWN]       = CAN_SHIFT_EXEC,
	[SHIFT_EXEC]                 = CAN_SHIFT_EXEC,
	[SHIFT_VALIDATE]             = CAN_SHIFT_EXEC,
	[SHIFT_FAILURE_WDG_ELAPSED]  = CAN_SHIFT_FAILURE_WDG_ELAPSED,
	[SHIFT_SUCCESS]              = CAN_SHIFT_SUCCESS,
	[SHIFT_GEAR_UNKNOWN]         = CAN_GEARSENS_UNKNOWN,
	[SHIFT_GEARSENS_FAILURE]     = CAN_GEARSENS_FAILURE,
	[SHIFT_GEARSENS_IMPLAUSIBLE] = CAN_GEARSENS_IMPLAUSIBLE,
	[SHIFT_DISABLED]             = CAN_SHIFT_DISABLED,
	[SHIFT_STATES_COUNT]         = CAN_SHIFT_DISABLED
};

//! Gear servo configuration
static const ServoConfig gearServoConfig = {
		.limits = {
			 .degMin     = (DEG_DEFAULT - DEG_DOWN_MAX), //!< 80 deg down
		     .degDefault = DEG_DEFAULT,
			 .degMax     = (DEG_DEFAULT + DEG_UP_MAX) //!< 80 deg up
		},
		.pwmChannel = TIM_CHANNEL_3,
		.direction = SERVO_DIR_COUNTERCLOCKWISE
};

//! Main handler
static __IO GearControlHandler gearCtrl = {
		.servoConfig = &gearServoConfig,
		.servo = SERVO_GEAR_SHIFT,
		.gear = GEAR_INIT,
		.canShiftStatusMap = shiftCanMap
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline GearStates GearCtrlState_Init(void);
static void GearControl_StateMachine(void);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */

/**
 * @brief Initialization of the gear control state machine.
 * 
 * @return The next state of the gear control state machine.
 */
static inline GearStates GearCtrlState_Init(void)
{
	GearStates nextState = GEAR_INIT;

	/* Check sensor plausibility before initialization */
	if (GearSensor_GetPlausibility() == GEAR_SENS_PLAUSIBLE) {

		const GearSensorStatesEnum gearSens = GearSensor_GetState();

		/* Initialization will continue only if gear sensor gives valid reading */
		if (gearSens <= GEAR_SENS_6) {
			/* Enable gear shifting servo */
			if (Servo_EnableAndGoToDefaultPos(gearCtrl.servo) == ERROR_OK) {
				/* Gear sensor reading OK, proceed with normal operation */
				/* ! Gears sensor reading are mapped 1:1 to GearStates ! */
				/* ! Order is important, do not change ! */
				nextState = (GearStates)gearSens;

				/* Activate MicroSwitches Control */
				/* Start by debouncing LOW state (both switches LOW) */
				MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);
			} else {
				nextState = GEAR_SERVO_FAILURE;
			}
		}
	}

	return nextState;
}

/**
 * @brief The function is a state machine that handles the gear shifting process.
 */
static void GearControl_StateMachine(void)
{
	switch (gearCtrl.gear) {
		case GEAR_INIT:
			/* Init state is handled separately */
			gearCtrl.gear = GearCtrlState_Init();
			break;

		case GEAR_1:
		case GEAR_N:
		case GEAR_2:
		case GEAR_3:
		case GEAR_4:
		case GEAR_5:
		case GEAR_6:
			/* All other states representing gears use the same shift handler */
			gearCtrl.gear = ShiftControl_Handler();
			break;

		case GEAR_UNKNOWN:
		case GEAR_SENS_FAILURE:
		case GEAR_IMPLAUSIBLE:
			/* Recover gear state */
			/* Go to default position and call handler to recover gear from sensor reading */
			if (Servo_SetDefaultPos(gearCtrl.servo) == ERROR_OK) {
				gearCtrl.gear = ShiftControl_Handler();
			} else {
				gearCtrl.gear = GEAR_SERVO_FAILURE;
			}
			break;

		case GEAR_SERVO_FAILURE:
		case GEAR_DISABLED:
		case GEAR_COUNT:
		default:
			/* Gear shifting is disabled, unrecoverable without MCU reset */
			(void)Servo_SetDefaultPos(gearCtrl.servo);
			break;
	}
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

/**
 * @brief Initialization of the gear control module.
 * 
 * @param htim pointer to the timer that will be used to generate the PWM signal.
 * 
 * @return an error code.
 */
ErrorEnum GearControl_Init(TIM_HandleTypeDef *const htim)
{
	ErrorEnum err = ERROR_OK;

	/* Initialize statistics */
	SwTimerInitStats(&gearCtrl.timStats, SHIFT_TIME_STATS_MAX);

	/* Initialize Gear Requests module */
	err = GearRequest_Init();

	/* Initialize Shift Control module */
	if (err == ERROR_OK) {
		err = ShiftControl_Init(&gearCtrl.timStats);
	}

	/* Initialize CAN reporting module */
	if (err == ERROR_OK) {
		err = GearControlCAN_Init(&gearCtrl.timStats);
	}

	if (err == ERROR_OK) {
		NULL_ERR_CHECK2(err, htim, htim->Instance);

		if (err == ERROR_OK) {
			/* Gear Control servo PWM parameters */
			ServoPwmParams gearServoPwmParams = {
				.htim = htim,
				.PWM = &htim->Instance->CCR3
			};

			/* Initialize gear servo */
			err = Servo_Init(gearCtrl.servo, gearCtrl.servoConfig, gearServoPwmParams);
		}
	}

	return err;
}

/**
 * @brief Main process function that is called from the Scheduler.
 */
void GearControl_Process(void)
{
	/* Update CAN Status based on current shifting state */
	GearControlCAN_UpdateStatus(gearCtrl.canShiftStatusMap[ShiftControl_GetState()]);

	/* Gear Control state machine */
	GearControl_StateMachine();

	/* Gear watchdog process */
	GearWatchdog_Process();
}

/**
 * @brief This function returns the current gear state.
 * 
 * @return Current gear.
 */
GearStates GearControl_GetGear(void)
{
	return gearCtrl.gear;
}

/*
 * GearControl.c
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */
#include "CAN.h"
#include "GearControl.h"
#include "GearControlMap.h"
#include "InjectorsCut.h"
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
	SHIFT_INIT = 0U,
	SHIFT_IDLE = 1U,
	SHIFT_PROCEDURE_UP = 2U,
	SHIFT_PROCEDURE_DOWN = 3U,
	SHIFT_EXEC = 4U,
	SHIFT_VALIDATE = 5U,
	SHIFT_DONE = 6U
} GearShiftStates;

static const ServoConfig gearServoConfig = {
		.limits = {
			 .degMin     = DEG_DOWN_MAX, //!< 80 deg down
		     .degDefault = DEG_DEFAULT,
			 .degMax     = DEG_UP_MAX //!< 80 deg up
		},
		.pwmChannel = TIM_CHANNEL_3
};

typedef struct {
	bool_t requested;
	uint32_t servoDeg;
	GearStates expectedGear; //!< Expected next gear
	GearShiftStates shiftProcedure;   //!< Shifting procedure that will be executed after request is processed
} GearShiftRequest;

typedef struct {
	GearWatchdogType *const watchdog;                 //!< Gear shift watchdog
	WatchdogFailTrigger watchdogFail;
	const GearServoDegData *const servoDegMap; //!< Gear servo degrees mapping
	const ServoConfig *const servoConfig;      //!< Gear servo configuration
	GearStates gear;                           //!< Actual gear by estimation and sensor reading
	GearShiftRequest request;
	GearShiftStates shiftState;                //!< Shift state - describes dynamic behavior
	const ServoTypeEnum servo;                 //!< Servo Type
} GearControlHandler;

static void GearWatchdogElapsedTrigger(void);

static GearWatchdogType gearWdg = {
		.elapsedTrigger = GearWatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = 200U
};

static __IO GearControlHandler gearCtrl = {.watchdog = &gearWdg,
										   .servoDegMap = &GearShiftDegMap[0U],
		                                   .servoConfig = &gearServoConfig,
										   .servo = SERVO_GEAR_SHIFT,
										   .gear = GEAR_INIT,
										   .shiftState = SHIFT_INIT,
										   .request = {
												 .expectedGear = GEAR_DISABLED,
												 .requested = FALSE,
												 .servoDeg = DEG_DEFAULT,
												 .shiftProcedure = SHIFT_IDLE
												}
};

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */
static inline GearStates GearCtrlState_Init(void);
static inline GearStates GearCtrlState_Handler(void);

static inline void GearCtrl_SetRequest(__IO GearShiftRequest *const request, const uint32_t servoDeg, const GearStates expectedGear, const GearShiftStates shift);
static inline void GearCtrl_ResetRequest(__IO GearShiftRequest *const request);

static inline GearShiftStates GearCtrl_ShiftExecute(__IO GearShiftRequest *const request);
static inline GearShiftStates GearCtrl_ShiftProcedureUp(void);
static inline GearShiftStates GearCtrl_ShiftProcedureDown(void);
static inline GearShiftStates GearCtrl_ShiftProcessRequests(__IO GearShiftRequest *const request);
/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static void GearWatchdogElapsedTrigger(void)
{
	/* Watchdog elapsed - failed gear shift */
	gearCtrl.shiftState = SHIFT_IDLE;
	GearCtrl_ResetRequest(&gearCtrl.request);
	MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);

	// TODO Update CAN Tx Data
}

static inline void GearCtrl_SetRequest(__IO GearShiftRequest *const request, const uint32_t servoDeg, const GearStates expectedGear, const GearShiftStates shift)
{
	if (NULL_CHECK1(request)) {
		request->requested = TRUE;
		request->servoDeg = servoDeg;
		request->expectedGear = expectedGear;
		request->shiftProcedure = shift;
	}
}

static inline void GearCtrl_ResetRequest(__IO GearShiftRequest *const request)
{
	if (NULL_CHECK1(request)) {
		request->requested = FALSE;
		request->servoDeg = DEG_DEFAULT;
		request->expectedGear = GEAR_DISABLED;
		request->shiftProcedure = SHIFT_IDLE;
	}
}

/* Gear state Initialization handler */
static inline GearStates GearCtrlState_Init(void)
{
	GearStates nextState = GEAR_INIT;
	GearSensorStatesEnum gearSensState = GearSensor_GetState();

	if (gearSensState <= GEAR_SENS_6) {
		/* Enable gear shifting servo */
		if (Servo_EnableAndGoToDefaultPos(gearCtrl.servo) == ERROR_OK) {
			/* Gear sensor reading OK, proceed with normal operation */
			/* ! Gears sensor reading are mapped 1:1 to GearStates ! */
			/* ! Order is important, do not change ! */
			nextState = (GearStates)gearSensState;

			/* Activate MicroSwitches Control */
			/* Start by debouncing LOW state (both switches LOW) */
			MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);

			gearCtrl.shiftState = SHIFT_IDLE;
		} else {
			nextState = GEAR_DISABLED;
		}
	}

	return nextState;
}

static inline GearShiftStates GearCtrl_ShiftExecute(__IO GearShiftRequest *const request)
{
	/* Execute shift */
	GearShiftStates nextShiftState = SHIFT_VALIDATE;

	if ((NULL_CHECK1(request)) && (request->requested)) {
		/* Set servo pos and go to DISABLED if error occurs */
		if (Servo_SetPos(gearCtrl.servo, request->servoDeg) == ERROR_OK) {
			/* Set servo position triggered, start gear watchdog */
			GearWatchdog_Start(gearCtrl.watchdog);
		} else {
			/* Failure */
			nextShiftState = SHIFT_IDLE;
			gearCtrl.gear = GEAR_DISABLED;
			GearCtrl_ResetRequest(request);
			Servo_SetDefaultPos(gearCtrl.servo);
			Servo_Disable(gearCtrl.servo);
		}
	}

	return nextShiftState;
}

static inline GearShiftStates GearCtrl_ShiftProcessRequests(__IO GearShiftRequest *const request)
{
	/* Request gear shift if any new request is OK. */
	uint32_t servoDeg = DEG_DEFAULT;
	GearStates expectedGear = GEAR_DISABLED;
	GearShiftStates nextShiftState = SHIFT_IDLE;

	if ((NULL_CHECK1(request)) && (request->requested)) {
		const GearRequestEnum newRequest = GearRequest_Get();

		switch (newRequest) {
			case GEAR_REQUEST_SHIFT_DOWN:
				/* Set new request for down-shift */
				servoDeg = gearCtrl.servoDegMap[gearCtrl.gear].degreesDown;
				expectedGear = gearCtrl.servoDegMap[gearCtrl.gear].expGearDown;
				GearCtrl_SetRequest(request, servoDeg, expectedGear, SHIFT_PROCEDURE_DOWN);
				break;

			case GEAR_REQUEST_SHIFT_UP:
				/* Set new request for up-shift */
				servoDeg = gearCtrl.servoDegMap[gearCtrl.gear].degreesUp;
				expectedGear = gearCtrl.servoDegMap[gearCtrl.gear].expGearUp;
				GearCtrl_SetRequest(request, servoDeg, expectedGear, SHIFT_PROCEDURE_UP);
				break;

			case GEAR_REQUEST_SHIFT_N:
				/* Set new request for going to Neutral */
				servoDeg = gearCtrl.servoDegMap[gearCtrl.gear].degreesN;
				expectedGear = GEAR_N;
				GearCtrl_SetRequest(request, servoDeg, expectedGear, SHIFT_EXEC);
				break;

			default:
				/* Anything else is invalid, just ignore */
				break;
		}

		/* requested flag updated by SetRequest() function if request is valid */
		if (request->requested) {
			/* Go to next selected shift state on valid request */
			nextShiftState = request->shiftProcedure;
			/* Disable MicroSwitches - shift is being processed */
			MicroSwitch_SetControl(MS_CONTROL_DISABLED);
		}
	}

	return nextShiftState;
}

static inline GearShiftStates GearCtrl_ShiftProcedureUp(void)
{
	/* Shift up procedure - injectors cut | clutch slip */
	GearShiftStates nextShiftState = SHIFT_PROCEDURE_UP;

	/*
	 * TODO Shift up procedure.
	 */

	/* Trigger injectors cut */
	InjectorsCut_Trigger();
	(void)nextShiftState;

	return SHIFT_EXEC;
}

static inline GearShiftStates GearCtrl_ShiftProcedureDown(void)
{
	/* Shift down procedure - rev match | throttle blip | clutch slip */
	GearShiftStates nextShiftState = SHIFT_PROCEDURE_DOWN;

	/*
	 * TODO Shift down procedure.
	 */

	(void)nextShiftState;

	return SHIFT_EXEC;
}

/* Gear state handler, called every 1 ms */
static inline GearStates GearCtrlState_Handler(void)
{
	GearStates nextGear = gearCtrl.gear;

	switch (gearCtrl.shiftState) {
		case SHIFT_INIT:
			/* Nothing to do here, just go to IDLE */
			gearCtrl.shiftState = SHIFT_IDLE;
			break;

		case SHIFT_IDLE:
			/* Process shift requests, invalid requests will be ignored */
			gearCtrl.shiftState = GearCtrl_ShiftProcessRequests(&gearCtrl.request);
			break;

		case SHIFT_PROCEDURE_DOWN:
			/* Shifting DOWN procedure before engaging servo */
			gearCtrl.shiftState = GearCtrl_ShiftProcedureDown();
			break;

		case SHIFT_PROCEDURE_UP:
			/* Shifting UP procedure before engaging servo */
			gearCtrl.shiftState = GearCtrl_ShiftProcedureUp();
			break;

		case SHIFT_EXEC:
			/* Execute shift - engage servo to requested position */
			gearCtrl.shiftState = GearCtrl_ShiftExecute(&gearCtrl.request);
			break;

		case SHIFT_VALIDATE:
			// TODO Validate if gear shift was successful, feed watchdog
			break;

		case SHIFT_DONE:
			// TODO Finish or remove DONE state, feed watchdog
			GearCtrl_ResetRequest(&gearCtrl.request);
			gearCtrl.shiftState = SHIFT_IDLE;
			MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);
			break;

		default:
			break;
	}

	return nextGear;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum GearControl_Init(TIM_HandleTypeDef *const htim)
{
	ErrorEnum err = ERROR_OK;

	/* Initialize Gear Requests module */
	err = GearRequest_Init();

	/* Initialize Gear Watchdog */
	if (err == ERROR_OK) {
		err = GearWatchdog_Init(gearCtrl.watchdog);
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

			err = Servo_Init(gearCtrl.servo, gearCtrl.servoConfig, gearServoPwmParams);
		}
	}

	return err;
}

/* Gear control main process */
void GearControl_Process(void)
{
	switch (gearCtrl.gear) {
		case GEAR_INIT:
			/* Init state is handled separately */
			gearCtrl.gear = GearCtrlState_Init();
			break;

		default:
			/* All other states use the same handler */
			gearCtrl.gear = GearCtrlState_Handler();
			break;
	}

	GearWatchdog_Process();
}

GearStates GearControl_GetGear(void)
{
	return gearCtrl.gear;
}

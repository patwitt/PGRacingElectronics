/*
 * GearControl.c
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */
#include "CAN.h"
#include "GearControl.h"
#include "GearControlMap.h"
#include "GearControlCAN.h"
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

//! Shift states - dynamic action of gear-shift
typedef enum {
	SHIFT_INIT                 = 0U,
	SHIFT_IDLE                 = 1U,
	SHIFT_PROCEDURE_UP         = 2U,
	SHIFT_PROCEDURE_DOWN       = 3U,
	SHIFT_EXEC                 = 4U,
	SHIFT_VALIDATE             = 5U,
	SHIFT_FAILURE_ELAPSED      = 6U,
	SHIFT_SUCCESS              = 7U,
	SHIFT_GEAR_UNKNOWN         = 8U,
	SHIFT_GEARSENS_FAILURE     = 9U,
	SHIFT_GEARSENS_IMPLAUSIBLE = 10U,
	SHIFT_DISABLED             = 11U
} GearShiftStates;

static const CANShiftStatus shiftCanMap[SHIFT_DISABLED + 1U] = {
	[SHIFT_INIT]                 = CAN_SHIFT_INIT,
	[SHIFT_IDLE]                 = CAN_SHIFT_IDLE,
	[SHIFT_PROCEDURE_UP]         = CAN_SHIFT_EXEC,
	[SHIFT_PROCEDURE_DOWN]       = CAN_SHIFT_EXEC,
	[SHIFT_EXEC]                 = CAN_SHIFT_EXEC,
	[SHIFT_VALIDATE]             = CAN_SHIFT_EXEC,
	[SHIFT_FAILURE_ELAPSED]      = CAN_SHIFT_FAILURE_ELAPSED,
	[SHIFT_SUCCESS]              = CAN_SHIFT_SUCCESS,
	[SHIFT_GEAR_UNKNOWN]         = CAN_GEARSENS_UNKNOWN,
	[SHIFT_GEARSENS_FAILURE]     = CAN_GEARSENS_FAILURE,
	[SHIFT_GEARSENS_IMPLAUSIBLE] = CAN_GEARSENS_IMPLAUSIBLE,
	[SHIFT_DISABLED]             = CAN_SHIFT_DISABLED
};

//! Gear servo configuration
static const ServoConfig gearServoConfig = {
		.limits = {
			 .degMin     = (DEG_DEFAULT - DEG_DOWN_MAX), //!< 80 deg down
		     .degDefault = DEG_DEFAULT,
			 .degMax     = (DEG_DEFAULT + DEG_UP_MAX) //!< 80 deg up
		},
		.pwmChannel = TIM_CHANNEL_3
};

//! Gear shift request
typedef struct {
	bool_t requested;
	uint32_t servoDeg;
	GearStates expectedGear; //!< Expected next gear
	GearShiftStates shiftProcedure;   //!< Shifting procedure that will be executed after request is processed
} GearShiftRequest;

//! Gear Control main handler
typedef struct {
	GearWatchdogType *const watchdog;          //!< Gear shift watchdog
	const GearServoDegData *const servoDegMap; //!< Gear servo degrees mapping
	const ServoConfig *const servoConfig;      //!< Gear servo configuration
	GearStates gear;                           //!< Actual gear by estimation and sensor reading
	GearShiftRequest request;                  //!< Gear shift request
	GearShiftStates shiftState;                //!< Shift state - describes dynamic behavior
	const ServoTypeEnum servo;                 //!< Servo Type
	const CANShiftStatus *const canShiftStatusMap;
	const uint32_t gearDebounceMs;
	uint32_t debCnt;
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
												},
										  .debCnt = 0U,
										  .gearDebounceMs = 400U,
										  .canShiftStatusMap = shiftCanMap
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline GearStates GearCtrlState_Init(void);
static inline GearStates GearCtrlState_Handler(void);

static inline void GearCtrl_SetRequest(__IO GearShiftRequest *const request, const uint32_t servoDeg, const GearStates expectedGear, const GearShiftStates shift);
static inline void GearCtrl_ResetRequest(__IO GearShiftRequest *const request);

static inline GearShiftStates GearCtrl_ShiftExecute(__IO GearShiftRequest *const request);
static inline GearShiftStates GearCtrl_ShiftProcedureUp(void);
static inline GearShiftStates GearCtrl_ShiftProcedureDown(void);
static inline void GearCtrl_ShiftValidateSetGear(GearStates *const gear, __IO GearShiftRequest *const request);
static inline GearShiftStates GearCtrl_ShiftProcessRequests(__IO GearShiftRequest *const request);
static inline GearShiftStates GearCtrl_ShiftSuccessful(void);

static inline GearShiftStates GearCtrl_CheckGearAgainstSensor(GearStates *const estabilishedGear);
static inline GearShiftStates GearCtrl_GearMonitoring(GearStates *const estabilishedGear);
/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static void GearWatchdogElapsedTrigger(void)
{
	/* Watchdog elapsed - failed gear shift */
	GearCtrl_ResetRequest(&gearCtrl.request);
	MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);
	if (Servo_SetDefaultPos(gearCtrl.servo) == ERROR_OK) {
		gearCtrl.shiftState = SHIFT_FAILURE_ELAPSED;
	} else {
		gearCtrl.shiftState = GEAR_SERVO_FAILURE;
	}
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

	if (GearSensor_GetPlausibility() == GEAR_SENS_PLAUSIBLE) {
		/* Enable gear shifting servo */
		if (Servo_EnableAndGoToDefaultPos(gearCtrl.servo) == ERROR_OK) {
			/* Gear sensor reading OK, proceed with normal operation */
			/* ! Gears sensor reading are mapped 1:1 to GearStates ! */
			/* ! Order is important, do not change ! */
			nextState = (GearStates)GearSensor_GetState();

			/* Activate MicroSwitches Control */
			/* Start by debouncing LOW state (both switches LOW) */
			MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);

			gearCtrl.shiftState = SHIFT_IDLE;
		} else {
			nextState = GEAR_SERVO_FAILURE;
		}
	}

	return nextState;
}

static inline GearShiftStates GearCtrl_ShiftExecute(__IO GearShiftRequest *const request)
{
	/* Execute shift and go to validation step */
	GearShiftStates nextShiftState = SHIFT_VALIDATE;

	if ((NULL_CHECK1(request)) && (request->requested)) {
		/* Set servo pos and go to DISABLED if error occurs */
		if (Servo_SetPos(gearCtrl.servo, request->servoDeg) == ERROR_OK) {
			/* Set servo position triggered, start gear watchdog */
			GearWatchdog_Start(gearCtrl.watchdog);
		} else {
			/* servo Failure */
			nextShiftState = SHIFT_DISABLED;
			gearCtrl.gear = GEAR_SERVO_FAILURE;
		}
	}

	return nextShiftState;
}

static inline GearShiftStates GearCtrl_ShiftProcessRequests(__IO GearShiftRequest *const request)
{
	/* Request gear shift if any new request is valid */
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

	/* Go to exec */
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

	/* Go to exec */
	return SHIFT_EXEC;
}

static inline void GearCtrl_ShiftValidateSetGear(GearStates *const gear, __IO GearShiftRequest *const request)
{
	const GearStates gearSens = (GearStates)GearSensor_GetState();

	if ((NULL_CHECK1(request)) &&
		(gearSens == request->expectedGear)) {
		/* Successful gearshift */
		*gear = gearSens;
		GearWatchdog_Feed(&gearWdg);
		InjectorsCut_Finish();
		gearCtrl.shiftState = SHIFT_SUCCESS;
	} else {
		/* Gearshift still in progress.
		 * If it takes too long, gear watchdog will
		 * reset request and go back to IDLE */
	}
}

static inline GearShiftStates GearCtrl_ShiftSuccessful(void)
{
	GearShiftStates nextShiftState = SHIFT_IDLE;

	/* Successful request, should go back to IDLE */
	GearCtrl_ResetRequest(&gearCtrl.request);
	MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);

	if (Servo_SetDefaultPos(gearCtrl.servo) != ERROR_OK) {
		nextShiftState = GEAR_SERVO_FAILURE;
	}

	return nextShiftState;
}

static inline GearShiftStates GearCtrl_CheckGearAgainstSensor(GearStates *const estabilishedGear)
{
	GearShiftStates nextShiftState = gearCtrl.shiftState;

	/* Debouncing of estabilished gear vs sensor reading */
	const GearStates gearSensReading = (GearStates)GearSensor_GetState();
	const bool_t gearFailCondition = (gearSensReading != *estabilishedGear);

	const UtilsDebounceStatus debounce = Utils_Debounce(gearFailCondition,
			                                            &gearCtrl.debCnt,
											            gearCtrl.gearDebounceMs);

	switch (debounce) {
		case DEBOUNCE_CNT_ZERO:
		case DEBOUNCE_IN_PROGRESS:
			/* Debouncing OK or in progress */
			/* Estabilished gear is equal to sensor reading */
			break;

		case DEBOUNCE_EXCEEDED:
			/* Gear sensor reading is different than estabilished gear */
			if (nextShiftState == SHIFT_IDLE) {
				/* Go to failure state first to inform system about it */
				nextShiftState = SHIFT_GEARSENS_FAILURE;
				*estabilishedGear = GEAR_SENS_FAILURE;
				gearCtrl.debCnt = 0U;
			} else {
				/* Something went wrong - recover by
				 * setting gear from gear sensor reading and going to IDLE */
				*estabilishedGear = gearSensReading;
				nextShiftState = SHIFT_IDLE;
			}
			break;

		case DEBOUNCE_STATUS_CNT:
		default:
			/* gear sensor reading vs estabilished gear failure */
			nextShiftState = SHIFT_GEARSENS_FAILURE;
			*estabilishedGear = GEAR_SENS_FAILURE;
			gearCtrl.debCnt = 0U;
			break;
	}

	return nextShiftState;
}

static inline GearShiftStates GearCtrl_GearMonitoring(GearStates *const estabilishedGear)
{
	/* Keep old shifting state */
	GearShiftStates nextShiftState = gearCtrl.shiftState;

	const GearSensorStatesEnum gearSensReading = GearSensor_GetState();

	/* Check if reading is not unknown */
	if (gearSensReading != GEAR_SENS_UNKNOWN) {
		/* Check gear sensor reading plausibility */
		const GearSensorPlausibilityEnum gearSensPlaus = GearSensor_GetPlausibility();

		switch (gearSensPlaus) {
			case GEAR_SENS_PLAUSIBILITY_IN_PROGRESS:
				/* Wait for plausibility debouncing to finish */
				break;

			case GEAR_SENS_PLAUSIBLE:
				/* Gear is plausible, continue gear monitoring */
				nextShiftState = GearCtrl_CheckGearAgainstSensor(estabilishedGear);
				break;

			case GEAR_SENS_PLAUSIBILITY_CNT:
			case GEAR_SENS_IMPLAUSIBLE:
			default:
				/* Sensor reading is implausible, disable gear shifting
				 * until sensor reading is plausible again */
				nextShiftState = SHIFT_GEARSENS_IMPLAUSIBLE;
				*estabilishedGear = GEAR_IMPLAUSIBLE;
				break;
		}
	} else {
		nextShiftState = SHIFT_GEAR_UNKNOWN;
		*estabilishedGear = GEAR_UNKNOWN;
	}

	return nextShiftState;
}

/* Gear state handler, called every 1 ms */
static inline GearStates GearCtrlState_Handler(void)
{
	GearStates nextGear = gearCtrl.gear;

	switch (gearCtrl.shiftState) {
		case SHIFT_INIT:
		case SHIFT_FAILURE_ELAPSED:
			/* Nothing to do here, just go to IDLE */
			gearCtrl.shiftState = SHIFT_IDLE;
			break;

		case SHIFT_IDLE:
			/* Gear sensor monitoring */
			gearCtrl.shiftState = GearCtrl_GearMonitoring(&nextGear);

			/* Process shift requests if monitoring is OK
			 * invalid requests will be ignored */
			if (gearCtrl.shiftState == SHIFT_IDLE) {
				gearCtrl.shiftState = GearCtrl_ShiftProcessRequests(&gearCtrl.request);
			}
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
			/* Execute shift - engage servo to the requested position */
			gearCtrl.shiftState = GearCtrl_ShiftExecute(&gearCtrl.request);
			break;

		case SHIFT_VALIDATE:
			/* Validate gearshift, feed watchdog */
			GearCtrl_ShiftValidateSetGear(&nextGear, &gearCtrl.request);
			break;

		case SHIFT_SUCCESS:
			/* Successful gearshift */
			gearCtrl.shiftState = GearCtrl_ShiftSuccessful();
			break;

		case SHIFT_GEAR_UNKNOWN:
		case SHIFT_GEARSENS_IMPLAUSIBLE:
		case SHIFT_GEARSENS_FAILURE:
			/* This will poll gear sensor until it gives valid gear reading */
			gearCtrl.shiftState = GearCtrl_GearMonitoring(&nextGear);
			break;

		case SHIFT_DISABLED:
		default:
			/* Do nothing */
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

	/* Initialize CAN reporting module */
	if (err == ERROR_OK) {
		err = GearControlCAN_Init();
	}

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
	/* Update CAN Status based on current shifting state */
	GearControlCAN_UpdateStatus(gearCtrl.canShiftStatusMap[gearCtrl.shiftState]);

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
			/* All other states use the same handler */
			gearCtrl.gear = GearCtrlState_Handler();
			break;

		case GEAR_UNKNOWN:
		case GEAR_SENS_FAILURE:
		case GEAR_IMPLAUSIBLE:
			/* Call handler to recover gear state */
			GearCtrl_ResetRequest(&gearCtrl.request);
			if (Servo_SetDefaultPos(gearCtrl.servo) == ERROR_OK) {
				gearCtrl.gear = GearCtrlState_Handler();
			} else {
				gearCtrl.gear = GEAR_SERVO_FAILURE;
			}
			break;

		case GEAR_SERVO_FAILURE:
		case GEAR_DISABLED:
		case GEAR_COUNT:
		default:
			/* Gear shifting is disabled, unrecoverable without MCU reset */
			GearCtrl_ResetRequest(&gearCtrl.request);
			(void)Servo_SetDefaultPos(gearCtrl.servo);
			break;
	}

	/* Gear watchdog process */
	GearWatchdog_Process();
}

GearStates GearControl_GetGear(void)
{
	return gearCtrl.gear;
}

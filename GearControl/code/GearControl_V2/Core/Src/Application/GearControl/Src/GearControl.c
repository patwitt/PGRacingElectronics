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
#include "ClutchControl.h"
#include "ShiftRevMatch.h"
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
	SHIFT_INIT                 = 0U,  //!< Shift initialization state
	SHIFT_IDLE                 = 1U,  //!< Shift IDLE state - ready for new requests
	SHIFT_PROCEDURE_UP         = 2U,  //!< Gear up-shift procedure state
	SHIFT_PROCEDURE_DOWN       = 3U,  //!< Gear down-shift procedure state
	SHIFT_EXEC                 = 4U,  //!< Execute gearshift state
	SHIFT_VALIDATE             = 5U,  //!< Gear shift validation state - after gearshift execute
	SHIFT_FAILURE_WDG_ELAPSED  = 6U,  //!< Gear watchdog elapsed state - gearshift exceeded 200ms
	SHIFT_SUCCESS              = 7U,  //!< Gear shift successful state
	SHIFT_GEAR_UNKNOWN         = 8U,  //!< Gear unknown state - sensor stuck between gears
	SHIFT_GEARSENS_FAILURE     = 9U,  //!< Gear sensor failure state
	SHIFT_GEARSENS_IMPLAUSIBLE = 10U, //!< Gear sensor is implausible state
	SHIFT_DISABLED             = 11U, //!< Shifting is disabled state
	SHIFT_STATES_COUNT         = 12U
} GearShiftStates;

//! Gear shift request
typedef struct {
	bool_t requested;               //!< New gear-shift request flag
	uint32_t servoDeg;              //!< Servo degrees for gearshift
	GearStates expectedGear;        //!< Expected next gear
	GearShiftStates shiftProcedure; //!< Shifting procedure that will be executed after request is processed
} GearShiftRequest;

//! Gear Control main handler struct
typedef struct {
	GearWatchdogType *const watchdog;               //!< Gear shift watchdog
	const GearServoDegData *const servoDegMap;      //!< Gear servo degrees mapping
	const ClutchSlipConfig *const upShiftSlipCfg;   //!< Clutch upshift slip config
	const ClutchSlipConfig *const downShiftSlipCfg; //!< Clutch downshift slip config
	const ServoConfig *const servoConfig;           //!< Gear servo configuration
	GearStates gear;                                //!< Actual gear by estimation and sensor reading
	GearShiftRequest request;                       //!< Gear shift request
	GearShiftStates shiftState;                     //!< Shift state - describes dynamic behavior
	const ServoEntityEnum servo;                    //!< Servo Entity Type
	const CANShiftStatus *const canShiftStatusMap;  //!< CAN Bus shift mapping - CAN signal <- shift status
	const uint32_t gearSensDebounceMs;              //!< Gear validation debouncing against sensor reading in ms
	uint32_t debCnt;                                //!< Debounce counter
	uint32_t delayTim;                              //!< Delay timer
	uint32_t clutchTimeout;
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

//! Up shift clutch slip configuration
static const ClutchSlipConfig upShiftClutchSlipConfig = {
	.slipDeg = CLUTCH_UPSHIFT_SLIP_DEG,
	.slipDelayMs = CLUTCH_UPSHIFT_DELAY_MS,
	.direction = CLUTCH_DIR_UPSHIFT
};

//! Down shift clutch slip configuration
static const ClutchSlipConfig downShiftClutchSlipConfig = {
	.slipDeg = CLUTCH_DOWNSHIFT_SLIP_DEG,
	.slipDelayMs = CLUTCH_DOWNSHIFT_DELAY_MS,
	.direction = CLUTCH_DIR_DOWNSHIFT
};

//! This must be before gearWdg declaration
static void GearWatchdogElapsedTrigger(void);

//! Gear watchdog
static GearWatchdogType gearWdg = {
		.elapsedTrigger = GearWatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = GEAR_SHIFT_TIMEOUT_MS
};

//! Main handler
static __IO GearControlHandler gearCtrl = {
		.watchdog = &gearWdg,
		.upShiftSlipCfg = &upShiftClutchSlipConfig,
		.downShiftSlipCfg = &downShiftClutchSlipConfig,
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
		.gearSensDebounceMs = 10U,
		.canShiftStatusMap = shiftCanMap,
		.delayTim = 0U
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline GearStates GearCtrlState_Init(void);
static inline GearStates GearCtrlState_ShiftHandler(void);

static inline void GearCtrl_SetRequest(__IO GearShiftRequest *const request,
		                               const uint32_t servoDeg,
									   const GearStates expectedGear,
									   const GearShiftStates shiftProcedure);
static inline void GearCtrl_ResetRequest(__IO GearShiftRequest *const request);

static void GearControl_StateMachine(void);
static inline GearShiftStates GearCtrl_ShiftExecute(__IO GearShiftRequest *const request);
static inline GearShiftStates GearCtrl_ShiftProcedureUp(void);
static inline GearShiftStates GearCtrl_ShiftProcedureDown(__IO GearShiftRequest *const request);
static inline GearShiftStates GearCtrl_ShiftValidateSetGear(GearStates *const gear, __IO GearShiftRequest *const request);
static inline GearShiftStates GearCtrl_ShiftProcessRequests(__IO GearShiftRequest *const request);
static inline GearShiftStates GearCtrl_ShiftSuccessful(void);

static inline GearShiftStates GearCtrl_CheckGearAgainstSensor(GearStates *const establishedGear);
static inline GearShiftStates GearCtrl_GearMonitoring(GearStates *const establishedGear);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */

/**
 * @brief Watchdog elapsed - failed gear shift.
 */
static void GearWatchdogElapsedTrigger(void)
{
	/* Reset request and debounce microswitches LOW */
	GearCtrl_ResetRequest(&gearCtrl.request);
	MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);

	/* Go back to default position and to failed gearshift state */
	if (Servo_SetDefaultPos(gearCtrl.servo) == ERROR_OK) {
		gearCtrl.shiftState = SHIFT_FAILURE_WDG_ELAPSED;
	} else {
		gearCtrl.shiftState = GEAR_SERVO_FAILURE;
	}
}

/**
 * @brief Set the gear shift request to the given parameters.
 * 
 * @param GearShiftRequest Gear shift request struct pointer.
 * @param servoDeg         The angle of the servo to move to.
 * @param expectedGear     The gear that the gearbox should be in after the shift.
 * @param shiftProcedure   Shift procedure state (up/down) that will be executed.
 */
static inline void GearCtrl_SetRequest(__IO GearShiftRequest *const request,
		                               const uint32_t servoDeg,
									   const GearStates expectedGear,
									   const GearShiftStates shiftProcedure)
{
	if (NULL_CHECK1(request)) {
		request->requested = TRUE;
		request->servoDeg = servoDeg;
		request->expectedGear = expectedGear;
		request->shiftProcedure = shiftProcedure;
		gearCtrl.delayTim = 0U;
	}
}

/**
 * @brief Resets the gear shift request.
 * 
 * @param GearShiftRequest Gear shift request struct pointer.
 */
static inline void GearCtrl_ResetRequest(__IO GearShiftRequest *const request)
{
	if (NULL_CHECK1(request)) {
		request->requested = FALSE;
		request->servoDeg = DEG_DEFAULT;
		request->expectedGear = GEAR_DISABLED;
		request->shiftProcedure = SHIFT_IDLE;
		gearCtrl.delayTim = 0U;
	}
}

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

				gearCtrl.shiftState = SHIFT_IDLE;
			} else {
				nextState = GEAR_SERVO_FAILURE;
			}
		}
	}

	return nextState;
}

/**
 * @brief Execute the shift request by setting the servo position and starting the gear watchdog.
 * 
 * @param GearShiftRequest Gear shift request struct pointer.
 * 
 * @return The next state of the gear shift state machine.
 */
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
			/* servo failure */
			nextShiftState = SHIFT_DISABLED;
			gearCtrl.gear = GEAR_SERVO_FAILURE;
		}
	}

	return nextShiftState;
}

/**
 * @brief Poll and process new requests in IDLE state.
 *
 * It checks if there's a new gear shift request, and if so, it sets the request parameters and returns
 * the next state to go to.
 * 
 * @param GearShiftRequest Gear shift request struct pointer.
 * 
 * @return The next state of the gear shift state machine.
 */
static inline GearShiftStates GearCtrl_ShiftProcessRequests(__IO GearShiftRequest *const request)
{
	/* Request gear shift if any new request is valid */
	uint32_t servoDeg = DEG_DEFAULT;
	GearStates expectedGear = GEAR_DISABLED;
	GearShiftStates nextShiftState = SHIFT_IDLE;

	if ((NULL_CHECK1(request)) && (!request->requested)) {
		const GearRequestEnum newRequest = GearRequest_Get();

		switch (newRequest) {
			case GEAR_REQUEST_SHIFT_DOWN:
				/* Set new request for down-shift */
				servoDeg = gearCtrl.servoDegMap[gearCtrl.gear].degreesDown;
				expectedGear = gearCtrl.servoDegMap[gearCtrl.gear].expGearDown;
				GearCtrl_SetRequest(request, servoDeg, expectedGear, SHIFT_EXEC);
				break;

			case GEAR_REQUEST_SHIFT_UP:
				/* Set new request for up-shift */
				servoDeg = gearCtrl.servoDegMap[gearCtrl.gear].degreesUp;
				expectedGear = gearCtrl.servoDegMap[gearCtrl.gear].expGearUp;
				GearCtrl_SetRequest(request, servoDeg, expectedGear, SHIFT_EXEC);
				break;

			case GEAR_REQUEST_SHIFT_N:
				/* Set new request for going to Neutral */
				servoDeg = gearCtrl.servoDegMap[gearCtrl.gear].degreesN;
				expectedGear = GEAR_N;
				GearCtrl_SetRequest(request, servoDeg, expectedGear, SHIFT_EXEC);
				break;

			case GEAR_REQUEST_NONE:
			case GEAR_REQUEST_INVALID:
			default:
				/* Anything else is irrelevant, just ignore */
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

/**
 * @brief Up-shift procedure.
 *
 * Trigger the clutch slip and injectors cut and if the clutch slip delay has elapsed, 
 * go to the next state.
 * 
 * The first state triggers the clutch slip and injectors cut, and then goes to the second state.
 * The second state waits for the clutch slip delay and injectors cut delay to elapse,
 * and then goes to execute gearshift state.
 * 
 * @return The next state of the state machine.
 */
static inline GearShiftStates GearCtrl_ShiftProcedureUp(void)
{
	typedef enum {
		SHIFT_PROCEDURE_UP_TRIGGERS,
		SHIFT_PROCEDURE_UP_DELAY
	} ShiftProcedureUpStates;

	static ShiftProcedureUpStates upProcedureState = SHIFT_PROCEDURE_UP_TRIGGERS;

	/* Stay in procedure up state until delay will be elapsed */
	GearShiftStates nextShiftState = SHIFT_PROCEDURE_UP;

	switch (upProcedureState) {
		case SHIFT_PROCEDURE_UP_TRIGGERS:
			/* Clutch Slip */
			if (ClutchControl_TriggerSlip(gearCtrl.upShiftSlipCfg->slipDeg,
					                        gearCtrl.upShiftSlipCfg->direction) == ERROR_OK) {
				gearCtrl.clutchTimeout = gearCtrl.upShiftSlipCfg->slipDelayMs;
			} else {
				gearCtrl.clutchTimeout = 0U;
			}

			/* Injectors Cut */
			InjectorsCut_Trigger();
			/* Go to delay */
			upProcedureState = SHIFT_PROCEDURE_UP_DELAY;
			break;

		case SHIFT_PROCEDURE_UP_DELAY:
		default:
			/* Tick delay timer */
			SwTimerDelay_Tick(&gearCtrl.delayTim);

			/* Check if both clutch slip delay and injectors cut delay has elapsed */
			if (SwTimerDelay_Elapsed(&gearCtrl.delayTim, gearCtrl.clutchTimeout) &&
			    SwTimerDelay_Elapsed(&gearCtrl.delayTim, INJECTORS_CUT_TRIGGER_DELAY_MS)) {
				upProcedureState = SHIFT_PROCEDURE_UP_TRIGGERS;
				nextShiftState = SHIFT_EXEC;
				gearCtrl.delayTim = 0U;
				gearCtrl.clutchTimeout = 0U;
			}
			break;
	}

	return nextShiftState;
}

/**
 * @brief Down-shift procedure.
 *
 * Trigger the clutch slip and if the clutch slip delay has elapsed, 
 * go to the next state.
 * 
 * The first state triggers the clutch slip, and then goes to the second state.
 * The second state waits for the clutch slip delay to elapse,
 * and then goes to execute gearshift state.
 * 
 * @return The next state of the state machine.
 */
static inline GearShiftStates GearCtrl_ShiftProcedureDown(__IO GearShiftRequest *const request)
{
	/* Shift down procedure - TODO add rev match */
	typedef enum {
		SHIFT_PROCEDURE_DOWN_TRIGGERS,
		SHIFT_PROCEDURE_DOWN_DELAY,
		SHIFT_PROCEDURE_DOWN_REVMATCH
	} ShiftProcedureDownStates;

	static ShiftProcedureDownStates downProcedureState = SHIFT_PROCEDURE_DOWN_TRIGGERS;

	/* Stay in procedure down state until delay will be elapsed */
	GearShiftStates nextShiftState = SHIFT_PROCEDURE_DOWN;

	switch (downProcedureState) {
		case SHIFT_PROCEDURE_DOWN_TRIGGERS:
			/* Clutch Slip */
			if (ClutchControl_TriggerSlip(gearCtrl.downShiftSlipCfg->slipDeg,
					                      gearCtrl.downShiftSlipCfg->direction) == ERROR_OK) {
				/* Go to delay */
				downProcedureState = SHIFT_PROCEDURE_DOWN_DELAY;
			} else {
				/* Clutch slip fail, exec gear */
				nextShiftState = SHIFT_EXEC;
			}
			break;

		case SHIFT_PROCEDURE_DOWN_DELAY:
			/* Tick delay timer */
			SwTimerDelay_Tick(&gearCtrl.delayTim);

			/* Check if clutch slip delay has elapsed */
			if (SwTimerDelay_Elapsed(&gearCtrl.delayTim, gearCtrl.downShiftSlipCfg->slipDelayMs)) {
				downProcedureState = SHIFT_PROCEDURE_DOWN_REVMATCH;
				gearCtrl.delayTim = 0U;
			}
			break;

		case SHIFT_PROCEDURE_DOWN_REVMATCH:
			/* Trigger revmatch */
			ShiftRevMatch_Trigger(request->expectedGear);
			downProcedureState = SHIFT_PROCEDURE_DOWN_TRIGGERS;
			nextShiftState = SHIFT_EXEC;
			break;

		default:
			/* Do nothing */
			break;
	}

	return nextShiftState;
}

/**
 * @brief Validation of gear shift after execute.
 * 
 * If the gear sensor is in the expected gear, set the current gear to the expected gear, feed the gear
 * watchdog, disable the injectors cut, reset the clutch slip, and change the state to success.
 * 
 * @param gear             pointer to the current gear.
 * @param GearShiftRequest Gear shift request struct pointer.
 */
static inline GearShiftStates GearCtrl_ShiftValidateSetGear(GearStates *const gear, __IO GearShiftRequest *const request)
{
	GearShiftStates nextShiftState = SHIFT_VALIDATE;
	const GearStates gearSens = (GearStates)GearSensor_GetState();

	if (gearSens == request->expectedGear) {
		/* Successful gear shift */
		/* Set current gear to expected one */
		*gear = request->expectedGear;
		/* Feed Gear Watchdog */
		GearWatchdog_Feed(&gearWdg);
		/* Disable Injectors Cut */
		InjectorsCut_Finish();
		/* Reset request */
		GearCtrl_ResetRequest(&gearCtrl.request);
		/* Go to default position */
		if (Servo_SetDefaultPos(gearCtrl.servo) == ERROR_OK) {
			/* Change state to SUCCESS */
			nextShiftState = SHIFT_SUCCESS;
		} else {
			nextShiftState = GEAR_SERVO_FAILURE;
		}
	} else {
		/* Gearshift still in progress.
		 * If it takes too long, gear watchdog will
		 * reset request and go back to IDLE */
	}

	return nextShiftState;
}

/**
 * @brief Successful gearshift state.
 *
 * Reset the request and set the microswitch control to debounce low.
 *
 * @return The next state of the gear shift state machine.
 */
static inline GearShiftStates GearCtrl_ShiftSuccessful(void)
{
	GearShiftStates nextShiftState = SHIFT_SUCCESS;

	/* MicroSwitches debounce LOW */
	MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);

	if (ShiftRevMatch_IsFinished()) {
		/* Reset clutch slip */
		ClutchControl_DisableSlip();
		/* Go to IDLE */
		nextShiftState = SHIFT_IDLE;
	}

	return nextShiftState;
}

/**
 * @brief Debounce the gear sensor reading against the established gear and if the debounce fails, go to the
 * failure state.
 * 
 * The function is called from the main state machine and it's purpose is to check if the gear sensor
 * reading is the same as the established gear. If it is, then the function returns the current state.
 * If it isn't, then the function returns the failure state.
 * 
 * @param establishedGear This is the gear that the system has established.
 * 
 * @return The nextShiftState is being returned.
 */
static inline GearShiftStates GearCtrl_CheckGearAgainstSensor(GearStates *const establishedGear)
{
	GearShiftStates nextShiftState = gearCtrl.shiftState;

	/* Debouncing of established gear vs sensor reading */
	const GearStates gearSensReading = (GearStates)GearSensor_GetState();
	const bool_t gearFailCondition = (gearSensReading != *establishedGear);

	const UtilsDebounceStatus debounce = Utils_Debounce(gearFailCondition,
			                                            &gearCtrl.debCnt,
											            gearCtrl.gearSensDebounceMs);

	switch (debounce) {
		case DEBOUNCE_CNT_ZERO:
		case DEBOUNCE_IN_PROGRESS:
			/* Debouncing OK or in progress */
			/* Established gear is equal to sensor reading */
			break;

		case DEBOUNCE_EXCEEDED:
			/* Something went wrong - recover by
			 * setting gear from gear sensor reading and going to IDLE */
			*establishedGear = gearSensReading;
			nextShiftState = SHIFT_IDLE;
			break;

		case DEBOUNCE_STATUS_CNT:
		default:
			/* gear sensor reading vs established gear failure */
			nextShiftState = SHIFT_GEARSENS_FAILURE;
			*establishedGear = GEAR_SENS_FAILURE;
			gearCtrl.debCnt = 0U;
			break;
	}

	return nextShiftState;
}

/**
 * @brief Gear shifting monitoring.
 * 
 * This function will recover wrong gear state if sensor reading is in valid range.
 * If the gear sensor reading is plausible, check the gear against the sensor reading.
 * Otherwise, return the shift state that indicates the gear sensor reading is
 * implausible.
 * 
 * The function is called from the main state machine, and the established gear is passed in as a
 * pointer. The function returns the next shift state.
 * 
 * @param establishedGear This is the gear that the car is currently in.
 * 
 * @return The nextShiftState is being returned.
 */
static inline GearShiftStates GearCtrl_GearMonitoring(GearStates *const establishedGear)
{
	/* Keep old shifting state */
	GearShiftStates nextShiftState = gearCtrl.shiftState;

	/* Check if reading is not unknown */
	if (GearSensor_GetState() != GEAR_SENS_UNKNOWN) {
		/* Check gear sensor reading plausibility */
		const GearSensorPlausibilityEnum gearSensPlaus = GearSensor_GetPlausibility();

		switch (gearSensPlaus) {
			case GEAR_SENS_PLAUSIBILITY_IN_PROGRESS:
				/* Wait for plausibility debouncing to finish */
				break;

			case GEAR_SENS_PLAUSIBLE:
				/* Gear is plausible, continue gear monitoring */
				nextShiftState = GearCtrl_CheckGearAgainstSensor(establishedGear);
				break;

			case GEAR_SENS_PLAUSIBILITY_CNT:
			case GEAR_SENS_IMPLAUSIBLE:
			default:
				/* Sensor reading is implausible, disable gear shifting
				 * until sensor reading is plausible again */
				nextShiftState = SHIFT_GEARSENS_IMPLAUSIBLE;
				*establishedGear = GEAR_IMPLAUSIBLE;
				break;
		}
	} else {
		nextShiftState = SHIFT_GEAR_UNKNOWN;
		*establishedGear = GEAR_UNKNOWN;
	}

	return nextShiftState;
}

/**
 * @brief State machine that handles gear shifting.
 * 
 * @return Next Gear that will be set.
 */
static inline GearStates GearCtrlState_ShiftHandler(void)
{
	GearStates nextGear = gearCtrl.gear;

	switch (gearCtrl.shiftState) {
		case SHIFT_INIT:
			/* Nothing to do here, just go to IDLE */
			gearCtrl.shiftState = SHIFT_IDLE;
			break;

		case SHIFT_IDLE:
			/* Shifting IDLE state, wait for new requests */
			gearCtrl.shiftState = GearCtrl_ShiftProcessRequests(&gearCtrl.request);
			break;

		case SHIFT_PROCEDURE_DOWN:
			/* Shifting DOWN procedure before engaging servo */
			gearCtrl.shiftState = GearCtrl_ShiftProcedureDown(&gearCtrl.request);
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
			gearCtrl.shiftState = GearCtrl_ShiftValidateSetGear(&nextGear, &gearCtrl.request);
			break;

		case SHIFT_SUCCESS:
			/* Successful gearshift */
			gearCtrl.shiftState = GearCtrl_ShiftSuccessful();
			break;

		case SHIFT_FAILURE_WDG_ELAPSED:
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
			gearCtrl.gear = GearCtrlState_ShiftHandler();
			break;

		case GEAR_UNKNOWN:
		case GEAR_SENS_FAILURE:
		case GEAR_IMPLAUSIBLE:
			/* Recover gear state */
			GearCtrl_ResetRequest(&gearCtrl.request);
			/* Go to default position and call handler to recover gear from sensor reading */
			if (Servo_SetDefaultPos(gearCtrl.servo) == ERROR_OK) {
				gearCtrl.gear = GearCtrlState_ShiftHandler();
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

	/* Initialize Rev match module */
	if (err == ERROR_OK) {
		err = ShiftRevMatch_Init();
	}

	/* Initialize injectors cut module */
	if (err == ERROR_OK) {
		err = InjectorsCut_Init();
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
	GearControlCAN_UpdateStatus(gearCtrl.canShiftStatusMap[gearCtrl.shiftState]);

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

/*
 * ShiftControl.c
 *
 *  Created on: 28.08.2022
 *      Author: Patryk Wittbrodt
 */
#include "CAN.h"
#include "GearControl.h"
#include "ShiftControl.h"
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

#define SHIFT_TIME_STATS_MAX (0xFFU)

//! Gear shift request
typedef struct {
	bool_t requested;           //!< New gear-shift request flag
	uint32_t servoDeg;          //!< Servo degrees for gearshift
	GearStates expectedGear;    //!< Expected next gear
	ShiftStates shiftProcedure; //!< Shifting procedure that will be executed after request is processed
} ShiftRequest;

//! Shift Control main handler struct
typedef struct {
	GearWatchdogType *const watchdog;
	const GearServoDegData *const servoDegMap;      //!< Gear servo degrees mapping
	const ClutchSlipConfig *const upShiftSlipCfg;   //!< Clutch upshift slip config
	const ClutchSlipConfig *const downShiftSlipCfg; //!< Clutch downshift slip config
	ShiftRequest request;                           //!< Gear shift request
	ShiftStates state;                              //!< Shift state - describes dynamic behavior
	const ServoEntityEnum servo;                    //!< Servo Entity Type
	const uint32_t gearSensDebounceMs;              //!< Gear validation debouncing against sensor reading in ms
	uint32_t debCnt;                                //!< Debounce counter
	uint32_t delayTim;                              //!< Delay timer
	uint32_t shiftTim;                              //!< Shift monitoring timer
	uint32_t clutchTimeout;                         //!< Clutch timeout, used for shifting procedure
	uint32_t gearTickStart;                         //!< Gear tick start for duration measurement
	__IO SwTimerStats* timStats;                    //!< Gear shifting statistics
} ShiftControlHandler;

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

//! Must be before shiftWdg declaration
static void ShiftControl_WatchdogElapsedTrigger(void);

//! Shifting watchdog
static GearWatchdogType shiftWdg = {
		.elapsedTrigger = ShiftControl_WatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = GEAR_SHIFT_TIMEOUT_MS
};

//! Main handler
static __IO ShiftControlHandler shiftCtrl = {
		.watchdog = &shiftWdg,
		.upShiftSlipCfg = &upShiftClutchSlipConfig,
		.downShiftSlipCfg = &downShiftClutchSlipConfig,
		.servoDegMap = &GearShiftDegMap[0U],
		.servo = SERVO_GEAR_SHIFT,
		.state = SHIFT_INIT,
		.request = {
				.expectedGear = GEAR_DISABLED,
				.requested = FALSE,
				.servoDeg = DEG_DEFAULT,
				.shiftProcedure = SHIFT_IDLE
		},
		.debCnt = 0U,
		.gearSensDebounceMs = 10U,
		.delayTim = 0U
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline void ShiftControl_SetRequest(__IO ShiftRequest *const request,
		                               const uint32_t servoDeg,
									   const GearStates expectedGear,
									   const ShiftStates shiftProcedure);
static inline void ShiftControl_ResetRequest(__IO ShiftRequest *const request);
static inline void ShiftControl_StateMonitoring(void);

static inline ShiftStates ShiftControl_Execute(__IO ShiftRequest *const request);
static inline ShiftStates ShiftControl_ProcedureUp(void);
static inline ShiftStates ShiftControl_ProcedureDown(__IO ShiftRequest *const request);
static inline ShiftStates ShiftControl_Validate(GearStates *const gear, __IO ShiftRequest *const request);
static inline ShiftStates ShiftControl_ProcessRequests(__IO ShiftRequest *const request);
static inline ShiftStates ShiftControl_Success(void);

static inline ShiftStates ShiftControl_CheckGearAgainstSensor(GearStates *const establishedGear);
static inline ShiftStates ShiftControl_GearMonitoring(GearStates *const establishedGear);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */

/**
 * @brief Watchdog has elapsed - failed gear shift.
 *
 * This will be triggered when max. shift duration has elapsed.
 * It resets request, informs system about unsuccessful shift
 * and goes back to IDLE.
 */

static void ShiftControl_WatchdogElapsedTrigger(void)
{
	/* Reset request and debounce microswitches LOW */
	ShiftControl_ResetRequest(&shiftCtrl.request);
	MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);

	SwTimerUpdateStats(shiftCtrl.timStats, SHIFT_TIME_STATS_MAX);

	/* Go back to default position and to failed gearshift state */
	if (Servo_SetDefaultPos(shiftCtrl.servo) == ERROR_OK) {
		shiftCtrl.state = SHIFT_FAILURE_WDG_ELAPSED;
	} else {
		shiftCtrl.state = GEAR_SERVO_FAILURE;
	}
}

/**
 * @brief Set the gear shift request to the given parameters.
 * 
 * @param ShiftRequest Gear shift request struct pointer.
 * @param servoDeg         The angle of the servo to move to.
 * @param expectedGear     The gear that the gearbox should be in after the shift.
 * @param shiftProcedure   Shift procedure state (up/down) that will be executed.
 */
static inline void ShiftControl_SetRequest(__IO ShiftRequest *const request,
		                               const uint32_t servoDeg,
									   const GearStates expectedGear,
									   const ShiftStates shiftProcedure)
{
	request->requested = TRUE;
	request->servoDeg = servoDeg;
	request->expectedGear = expectedGear;
	request->shiftProcedure = shiftProcedure;
	shiftCtrl.delayTim = 0U;
}

/**
 * @brief Resets the gear shift request.
 * 
 * @param ShiftRequest Gear shift request struct pointer.
 */
static inline void ShiftControl_ResetRequest(__IO ShiftRequest *const request)
{
	request->requested = FALSE;
	request->servoDeg = DEG_DEFAULT;
	request->expectedGear = GEAR_DISABLED;
	request->shiftProcedure = SHIFT_IDLE;
	shiftCtrl.delayTim = 0U;
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
			if (Servo_EnableAndGoToDefaultPos(shiftCtrl.servo) == ERROR_OK) {
				/* Gear sensor reading OK, proceed with normal operation */
				/* ! Gears sensor reading are mapped 1:1 to GearStates ! */
				/* ! Order is important, do not change ! */
				nextState = (GearStates)gearSens;

				/* Activate MicroSwitches Control */
				/* Start by debouncing LOW state (both switches LOW) */
				MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);

				shiftCtrl.state = SHIFT_IDLE;
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
 * @param ShiftRequest Gear shift request struct pointer.
 * 
 * @return The next state of the gear shift state machine.
 */
static inline ShiftStates ShiftControl_Execute(__IO ShiftRequest *const request)
{
	/* Execute shift and go to validation step */
	ShiftStates nextState = SHIFT_VALIDATE;

	/* Set servo pos and go to DISABLED if error occurs */
	if (Servo_SetPos(shiftCtrl.servo, request->servoDeg) == ERROR_OK) {
		/* Set servo position triggered, start gear watchdog */
		GearWatchdog_Start(shiftCtrl.watchdog);
	} else {
		/* servo failure */
		nextState = SHIFT_DISABLED;
	}

	return nextState;
}

/**
 * @brief Poll and process new requests in IDLE state.
 *
 * It checks if there's a new gear shift request, and if so, it sets the request parameters and returns
 * the next state to go to.
 *
 * @param ShiftRequest Gear shift request struct pointer.
 *
 * @return The next state of the gear shift state machine.
 */
static inline ShiftStates ShiftControl_ProcessRequests(__IO ShiftRequest *const request)
{
	/* Request gear shift if any new request is valid */
	uint32_t servoDeg = DEG_DEFAULT;
	GearStates expectedGear = GEAR_DISABLED;
	ShiftStates nextState = SHIFT_IDLE;

	const GearRequestEnum newRequest = GearRequest_Get();
	const uint8_t currentGear = (uint8_t)GearControl_GetGear();

	switch (newRequest) {
		case GEAR_REQUEST_SHIFT_DOWN:
			/* Set new request for down-shift */
			servoDeg = shiftCtrl.servoDegMap[currentGear].degreesDown;
			expectedGear = shiftCtrl.servoDegMap[currentGear].expGearDown;
			ShiftControl_SetRequest(request, servoDeg, expectedGear, SHIFT_EXEC);
			break;

		case GEAR_REQUEST_SHIFT_UP:
			/* Set new request for up-shift */
			servoDeg = shiftCtrl.servoDegMap[currentGear].degreesUp;
			expectedGear = shiftCtrl.servoDegMap[currentGear].expGearUp;
			ShiftControl_SetRequest(request, servoDeg, expectedGear, SHIFT_PROCEDURE_UP);
			break;

		case GEAR_REQUEST_SHIFT_N:
			/* Set new request for going to Neutral */
			servoDeg = shiftCtrl.servoDegMap[currentGear].degreesN;
			expectedGear = GEAR_N;
			ShiftControl_SetRequest(request, servoDeg, expectedGear, SHIFT_EXEC);
			break;

		case GEAR_REQUEST_NONE:
		case GEAR_REQUEST_INVALID:
		default:
			ShiftControl_ResetRequest(request);
			/* Anything else is irrelevant, just ignore */
			break;
	}

	/* requested flag updated by SetRequest() function if request is valid */
	if (request->requested) {
		shiftCtrl.gearTickStart = SwTimerGetUptime();
		/* Go to next selected shift state on valid request */
		nextState = request->shiftProcedure;
		/* Disable MicroSwitches - shift is being processed */
		MicroSwitch_SetControl(MS_CONTROL_DISABLED);
		/* Reset requested flag */
		request->requested = FALSE;
	}

	return nextState;
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
static inline ShiftStates ShiftControl_ProcedureUp(void)
{
	typedef enum {
		SHIFT_PROCEDURE_UP_TRIGGERS,
		SHIFT_PROCEDURE_UP_DELAY
	} ShiftProcedureUpStates;

	static ShiftProcedureUpStates upProcedureState = SHIFT_PROCEDURE_UP_TRIGGERS;

	/* Stay in procedure up state until delay will be elapsed */
	ShiftStates nextState = SHIFT_PROCEDURE_UP;

	switch (upProcedureState) {
		case SHIFT_PROCEDURE_UP_TRIGGERS:
#if 0
			/* Clutch Slip */
			if (ClutchControl_TriggerSlip(shiftCtrl.upShiftSlipCfg->slipDeg,
					                        shiftCtrl.upShiftSlipCfg->direction) == ERROR_OK) {
				shiftCtrl.clutchTimeout = shiftCtrl.upShiftSlipCfg->slipDelayMs;
			} else {
				shiftCtrl.clutchTimeout = 0U;
			}
#endif
			/* Injectors Cut */
			InjectorsCut_Trigger();
			/* Go to delay */
			upProcedureState = SHIFT_PROCEDURE_UP_DELAY;
			break;

		case SHIFT_PROCEDURE_UP_DELAY:
		default:
			/* Tick delay timer */
			SwTimerDelay_Tick(&shiftCtrl.delayTim);

			/* Check if both clutch slip delay and injectors cut delay has elapsed */
#if 0
			if (SwTimerDelay_Elapsed(&shiftCtrl.delayTim, shiftCtrl.clutchTimeout) &&
			    SwTimerDelay_Elapsed(&shiftCtrl.delayTim, INJECTORS_CUT_TRIGGER_DELAY_MS)) {
#endif
			if (SwTimerDelay_Elapsed(&shiftCtrl.delayTim, INJECTORS_CUT_TRIGGER_DELAY_MS)) {
				upProcedureState = SHIFT_PROCEDURE_UP_TRIGGERS;
				nextState = SHIFT_EXEC;
				shiftCtrl.delayTim = 0U;
				//shiftCtrl.clutchTimeout = 0U;
			}
			break;
	}

	return nextState;
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
static inline ShiftStates ShiftControl_ProcedureDown(__IO ShiftRequest *const request)
{
	/* Shift down procedure - TODO add rev match */
	typedef enum {
		SHIFT_PROCEDURE_DOWN_TRIGGERS,
		SHIFT_PROCEDURE_DOWN_DELAY,
		SHIFT_PROCEDURE_DOWN_REVMATCH
	} ShiftProcedureDownStates;

	static ShiftProcedureDownStates downProcedureState = SHIFT_PROCEDURE_DOWN_TRIGGERS;

	/* Stay in procedure down state until delay will be elapsed */
	ShiftStates nextState = SHIFT_PROCEDURE_DOWN;

	switch (downProcedureState) {
		case SHIFT_PROCEDURE_DOWN_TRIGGERS:
			/* Clutch Slip */
			if (ClutchControl_TriggerSlip(shiftCtrl.downShiftSlipCfg->slipDeg,
					                      shiftCtrl.downShiftSlipCfg->direction) == ERROR_OK) {
				/* Go to delay */
				downProcedureState = SHIFT_PROCEDURE_DOWN_DELAY;
			} else {
				/* Clutch slip fail, exec gear */
				nextState = SHIFT_EXEC;
			}
			break;

		case SHIFT_PROCEDURE_DOWN_DELAY:
			/* Tick delay timer */
			SwTimerDelay_Tick(&shiftCtrl.delayTim);

			/* Check if clutch slip delay has elapsed */
			if (SwTimerDelay_Elapsed(&shiftCtrl.delayTim, shiftCtrl.downShiftSlipCfg->slipDelayMs)) {
				downProcedureState = SHIFT_PROCEDURE_DOWN_REVMATCH;
				shiftCtrl.delayTim = 0U;
			}
			break;

		case SHIFT_PROCEDURE_DOWN_REVMATCH:
			/* Trigger revmatch */
			ShiftRevMatch_Trigger(request->expectedGear);
			downProcedureState = SHIFT_PROCEDURE_DOWN_TRIGGERS;
			nextState = SHIFT_EXEC;
			break;

		default:
			/* Do nothing */
			break;
	}

	return nextState;
}

/**
 * @brief Validation of gear shift after execute.
 * 
 * If the gear sensor is in the expected gear, set the current gear to the expected gear, feed the gear
 * watchdog, disable the injectors cut, reset the clutch slip, and change the state to success.
 * 
 * @param gear             pointer to the current gear.
 * @param ShiftRequest Gear shift request struct pointer.
 */
static inline ShiftStates ShiftControl_Validate(GearStates *const gear, __IO ShiftRequest *const request)
{
	ShiftStates nextState = SHIFT_VALIDATE;

	/* Validate gear shift */
	if ((GearStates)GearSensor_GetState() == request->expectedGear) {
		/* Set current gear to expected one */
		*gear = request->expectedGear;
		/* Feed Gear Watchdog */
		GearWatchdog_Feed(shiftCtrl.watchdog);
		/* Disable Injectors Cut */
		InjectorsCut_Finish();
		/* Reset request */
		ShiftControl_ResetRequest(&shiftCtrl.request);
		/* Go to default position */
		if (Servo_SetDefaultPos(shiftCtrl.servo) == ERROR_OK) {
			/* Measure shift duration */
			const uint32_t shiftDuration = SwTimerGetUptime() - shiftCtrl.gearTickStart;
			SwTimerUpdateStats(shiftCtrl.timStats, shiftDuration);
			/* Change state to SUCCESS */
			nextState = SHIFT_SUCCESS;
		} else {
			nextState = GEAR_SERVO_FAILURE;
		}
	} else {
		/* Gearshift still in progress.
		 * If it takes too long, gear watchdog will
		 * reset request and go back to IDLE */
	}

	return nextState;
}

/**
 * @brief Successful gearshift state.
 *
 * Reset the request and set the microswitch control to debounce low.
 *
 * @return The next state of the gear shift state machine.
 */
static inline ShiftStates ShiftControl_Success(void)
{
	ShiftStates nextState = SHIFT_SUCCESS;

	/* MicroSwitches debounce LOW */
	MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);

	if (ShiftRevMatch_IsFinished()) {
		/* Reset clutch slip */
		ClutchControl_DisableSlip();
		/* Go to IDLE */
		nextState = SHIFT_IDLE;
	}

	return nextState;
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
 * @return The nextState is being returned.
 */
static inline ShiftStates ShiftControl_CheckGearAgainstSensor(GearStates *const establishedGear)
{
	ShiftStates nextState = shiftCtrl.state;

	/* Debouncing of established gear vs sensor reading */
	const GearStates gearSensReading = (GearStates)GearSensor_GetState();
	const bool_t gearFailCondition = (gearSensReading != *establishedGear);

	const UtilsDebounceStatus debounce = Utils_Debounce(gearFailCondition,
			                                            &shiftCtrl.debCnt,
											            shiftCtrl.gearSensDebounceMs);

	switch (debounce) {
		case DEBOUNCE_CNT_ZERO:
		case DEBOUNCE_IN_PROGRESS:
			/* Debouncing OK or in progress */
			/* Established gear is equal to sensor reading */
			break;

		case DEBOUNCE_EXCEEDED:
			/* Gear sensor reading is different than established gear.
			 * Something went wrong - recover by
			 * setting gear from gear sensor reading and going to IDLE */
			*establishedGear = gearSensReading;
			nextState = SHIFT_IDLE;
			break;

		case DEBOUNCE_STATUS_CNT:
		default:
			/* gear sensor reading vs established gear failure */
			nextState = SHIFT_GEARSENS_FAILURE;
			*establishedGear = GEAR_SENS_FAILURE;
			shiftCtrl.debCnt = 0U;
			break;
	}

	return nextState;
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
 * @return The nextState is being returned.
 */
static inline ShiftStates ShiftControl_GearMonitoring(GearStates *const establishedGear)
{
	/* Keep old shifting state */
	ShiftStates nextState = shiftCtrl.state;

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
				nextState = ShiftControl_CheckGearAgainstSensor(establishedGear);
				break;

			case GEAR_SENS_PLAUSIBILITY_CNT:
			case GEAR_SENS_IMPLAUSIBLE:
			default:
				/* Sensor reading is implausible, disable gear shifting
				 * until sensor reading is plausible again */
				nextState = SHIFT_GEARSENS_IMPLAUSIBLE;
				*establishedGear = GEAR_IMPLAUSIBLE;
				break;
		}
	} else {
		nextState = SHIFT_GEAR_UNKNOWN;
		*establishedGear = GEAR_UNKNOWN;
	}

	return nextState;
}

/**
 * @brief Shift Handler state monitoring.
 * 
 * If the gear shift is not idle, tick the timer and if it has elapsed, reset the request and set the
 * microswitch control to debounce low. This will prevent system from freezing in non-idle state.
 */
static inline void ShiftControl_StateMonitoring(void)
{
#define SHIFT_TIMEOUT_MS (420U)

	switch (shiftCtrl.state) {
		case SHIFT_IDLE:
			/* Reset timer, shift state is IDLE */
			shiftCtrl.shiftTim = 0U;
			break;

		default:
			/* Tick timer */
			SwTimerDelay_Tick(&shiftCtrl.shiftTim);

			/* Go to IDLE if system is stuck in some non-idle state for too long */
			if (SwTimerDelay_Elapsed(&shiftCtrl.shiftTim, SHIFT_TIMEOUT_MS)) {
				ShiftControl_ResetRequest(&shiftCtrl.request);
				MicroSwitch_SetControl(MS_CONTROL_DEBOUNCE_LOW);
				shiftCtrl.state = SHIFT_IDLE;
				shiftCtrl.shiftTim = 0U;
			}
			break;
	}
}

static inline void ShiftControl_StateMachine(GearStates *const nextGear)
{
	switch (shiftCtrl.state) {
		case SHIFT_INIT:
			/* Wait for Gear Control Initialization before going to IDLE */
			if (GearControl_GetGear() != GEAR_INIT) {
				shiftCtrl.state = SHIFT_IDLE;
			}
			break;

		case SHIFT_IDLE:
			/* Shifting IDLE state, wait for new requests */
			shiftCtrl.state = ShiftControl_ProcessRequests(&shiftCtrl.request);
			break;

		case SHIFT_PROCEDURE_DOWN:
			/* Shifting DOWN procedure before engaging servo */
			shiftCtrl.state = ShiftControl_ProcedureDown(&shiftCtrl.request);
			break;

		case SHIFT_PROCEDURE_UP:
			/* Shifting UP procedure before engaging servo */
			shiftCtrl.state = ShiftControl_ProcedureUp();
			break;

		case SHIFT_EXEC:
			/* Execute shift - engage servo to the requested position */
			shiftCtrl.state = ShiftControl_Execute(&shiftCtrl.request);
			break;

		case SHIFT_VALIDATE:
			/* Validate gearshift, feed watchdog */
			shiftCtrl.state = ShiftControl_Validate(nextGear, &shiftCtrl.request);
			break;

		case SHIFT_SUCCESS:
			/* Successful gearshift */
			shiftCtrl.state = ShiftControl_Success();
			break;

		case SHIFT_FAILURE_WDG_ELAPSED:
		case SHIFT_GEAR_UNKNOWN:
		case SHIFT_GEARSENS_IMPLAUSIBLE:
		case SHIFT_GEARSENS_FAILURE:
			/* Reset request */
			ShiftControl_ResetRequest(&shiftCtrl.request);
			/* This will poll gear sensor until it gives valid gear reading */
			shiftCtrl.state = ShiftControl_GearMonitoring(nextGear);
			break;

		case SHIFT_DISABLED:
		default:
			/* Invalid or disabled */
			*nextGear = GEAR_DISABLED;
			break;
	}
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

ErrorEnum ShiftControl_Init(__IO SwTimerStats *const gearTimStats)
{
	ErrorEnum err = ERROR_OK;

	if (gearTimStats != NULL) {
		shiftCtrl.timStats = gearTimStats;
	} else {
		err = ERROR_NULL;
	}

	/* Initialize Gear shifting Watchdog */
	if (err == ERROR_OK) {
		err = GearWatchdog_Init(shiftCtrl.watchdog);
	}

	/* Initialize Rev match module */
	if (err == ERROR_OK) {
		err = ShiftRevMatch_Init();
	}

	/* Initialize injectors cut module */
	if (err == ERROR_OK) {
		err = InjectorsCut_Init();
	}

	return err;
}

ShiftStates ShiftControl_GetState(void)
{
	return shiftCtrl.state;
}

/**
 * @brief Shifting Handler.
 *
 * @return Next Gear that will be set.
 */
GearStates ShiftControl_Handler(void)
{
	GearStates nextGear = GearControl_GetGear();

	/* Monitor shift state to prevent freeze */
	ShiftControl_StateMonitoring();

	/* Shift State machine */
	ShiftControl_StateMachine(&nextGear);

	return nextGear;
}

/*
 * ClutchControl.c
 *
 *  Created on: 11.07.2022
 *      Author: Patryk Wittbrodt
 */

#include "GearWatchdog.h"
#include "ClutchControl.h"
#include "Servo.h"
#include "CAN.h"
#include "Utils.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

//! Clutch Position limits struct
typedef struct {
	const uint32_t degMin;
	const uint32_t degDefault;
	const uint32_t degMax;
} ClutchPosLimits;

//! Clutch Control handler struct
typedef struct {
	ClutchControlStates control;
	ClutchControlStates savedCtrl;
	GearWatchdogType *runningWdg;
	const ServoTypeEnum servo;
	const CAN_RxMsgEnum canRxMsg;
	bool_t canAck;
} ClutchControlHandler;

//! Clutch servo configuration
static const ServoConfig clutchServoConfig = {
		.limits = {
			 .degMin     = 18U,
		     .degDefault = 18U,
			 .degMax     = 162U
		},
		.pwmChannel = TIM_CHANNEL_1
};

//! This must be before watchdogs structs declaration
static void ClutchControl_WatchdogElapsedTrigger(void);

//! Upshift clutch slip watchdog
static GearWatchdogType upShiftClutchSlipWdg = {
		.elapsedTrigger = ClutchControl_WatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = CLUTCH_UPSHIFT_SLIP_TIMEOUT_MS
};

//! Downshift clutch slip watchdog
static GearWatchdogType downShiftClutchSlipWdg = {
		.elapsedTrigger = ClutchControl_WatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = CLUTCH_DOWNSHIFT_SLIP_TIMEOUT_MS
};

//! Mapping of shift direction <- running watchdog
static GearWatchdogType *const clutchWdgMap[CLUTCH_DIR_COUNT + 1U] = {
	[CLUTCH_DIR_UPSHIFT]   = &upShiftClutchSlipWdg,
	[CLUTCH_DIR_DOWNSHIFT] = &downShiftClutchSlipWdg,
	[CLUTCH_DIR_COUNT]     = NULL
};

//! Clutch Control handler
static ClutchControlHandler clutchCtrl = {
		.control  = CLUTCH_CTRL_INIT_CAN_ACK,
		.savedCtrl = CLUTCH_CTRL_INIT_CAN_ACK,
		.servo    = SERVO_CLUTCH,
		.canRxMsg = CAN_RX_MSG_CLUTCH,
		.canAck   = FALSE,
		.runningWdg = NULL
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static void ClutchControl_StateMachine(void);
static void ClutchControl_CAN_Acknowledge(void);
static void ClutchControl_CAN_NormalOperation(void);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */

/**
 * @brief Clutch watchdog elapsed function.
 * 
 * If the clutch is engaged, disengage it and set the clutch control to the saved control.
 */
static void ClutchControl_WatchdogElapsedTrigger(void)
{
	(void)Servo_SetDefaultPos(clutchCtrl.servo);
	clutchCtrl.control = clutchCtrl.savedCtrl;
}

/**
 * @brief Wait for CAN Acknowledge message state.
 *
 * If the CAN message is valid, and the message is an acknowledge message, then enable the servo and go
 * to normal operation mode.
 */
static void ClutchControl_CAN_Acknowledge(void)
{
	CAN_RxMsgType* clutchRxMsg = CAN_GetRxMsg(clutchCtrl.canRxMsg);

	/* Validate CAN RX message. If it's invalid, go to CAN ERROR state (default servo pos) */
	if (CAN_ValidateRxMsg(clutchRxMsg) == ERROR_OK) {
		if ((clutchRxMsg->newData) && (*clutchRxMsg->status == CAN_STATUS_ACK)) {
			/* CAN message received with acknowledge status */
			clutchCtrl.canAck = TRUE;

			/* Try to enable clutch servo */
			if (Servo_EnableAndGoToDefaultPos(clutchCtrl.servo) == ERROR_OK) {
				clutchCtrl.control = CLUTCH_CTRL_NORMAL_OP;
			} else {
				clutchCtrl.control = CLUTCH_CTRL_SERVO_DISABLED;
			}
		}
	}  else {
		clutchCtrl.control = CLUTCH_CTRL_CAN_ERROR;
	}
}

/**
 * @brief Normal Operation of Clutch Control (from steering wheel).
 * 
 * If the CAN message is valid, set the servo position to the value received from the CAN message.
 * The first thing we do is get the CAN message from the CAN driver. 
 * Then we validate the CAN message. If the message is invalid, we go to the CAN ERROR state. 
 * If the message is valid, we check if the message is new and if we're allowed to acknowledge the
 * message. 
 * If the message is new and we're allowed to acknowledge it, we get the clutch position from the CAN
 * message. 
 * Then we set the servo position to the value received from the CAN message. 
 * Finally, we set the newData flag to false and the CAN message is ready to receive a new message.
 */
static void ClutchControl_CAN_NormalOperation(void)
{
	CAN_RxMsgType* clutchRxMsg = CAN_GetRxMsg(clutchCtrl.canRxMsg);

	/* Validate CAN RX message. If it's invalid, go to CAN ERROR state (default servo pos) */
	if (CAN_ValidateRxMsg(clutchRxMsg) == ERROR_OK) {
		if ((clutchRxMsg->newData) && (clutchCtrl.canAck)) {

		/* Process clutch position from CAN */
		const uint32_t clutchPosFromCan = (uint32_t)clutchRxMsg->buffer[CAN_DATA_BYTE_DATA_0];

		/* Set received servo position */
		if (Servo_SetPos(clutchCtrl.servo, clutchPosFromCan) != ERROR_OK) {
			clutchCtrl.control = CLUTCH_CTRL_SERVO_DISABLED;
		}

		clutchRxMsg->newData = FALSE;
		}
	} else {
		clutchCtrl.control = CLUTCH_CTRL_CAN_ERROR;
	}
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
/**
 * @brief Initialization of the clutch control module.
 * 
 * @param htim pointer to the timer used for the clutch servo.
 * 
 * @return an error code.
 */
ErrorEnum ClutchControl_Init(TIM_HandleTypeDef *const htim)
{
	ErrorEnum err = ERROR_OK;

	NULL_ERR_CHECK2(err, htim, htim->Instance);

	if (err == ERROR_OK) {
		/* Clutch servo PWM parameters */
		ServoPwmParams clutchServoPwmParams = {
			.htim = htim,
			.PWM = &htim->Instance->CCR1
		};

		/* Initialize Clutch servo */
		err = Servo_Init(clutchCtrl.servo, &clutchServoConfig, clutchServoPwmParams);

		/* Initialize watchdog for clutch slip upshifts */
		if (err == ERROR_OK) {
			err = GearWatchdog_Init(&upShiftClutchSlipWdg);
		}

		/* Initialize watchdog for clutch slip downshifts */
		if (err == ERROR_OK) {
			err = GearWatchdog_Init(&downShiftClutchSlipWdg);
		}
	}

	return err;
}

/**
 * @brief Trigger the clutch slip procedure.
 *
 * It sets the clutch control to gear control, sets the servo position to the specified slip degrees,
 * and starts the watchdog.
 * 
 * @param slipDegrees The number of degrees to slip the clutch.
 * @param direction   CLUTCH_DIR_UP or CLUTCH_DIR_DOWN.
 */
void ClutchControl_TriggerSlip(const uint32_t slipDegrees, const ClutchShiftDirection direction)
{
	clutchCtrl.runningWdg = clutchWdgMap[direction];
	clutchCtrl.savedCtrl = clutchCtrl.control;

#if CONFIG_ENABLE_CLUTCH && CONFIG_ENABLE_CLUTCH_SLIP
	if (direction < CLUTCH_DIR_COUNT) {
		clutchCtrl.control = CLUTCH_CTRL_GEARCTRL;
		(void)Servo_SetPos(clutchCtrl.servo, slipDegrees);
		GearWatchdog_Start(clutchCtrl.runningWdg);
	}
#endif
}

/**
 * @brief Disable the clutch slip procedure.
 * 
 * If the clutch is enabled and the slip control is enabled, then set the servo to the default position
 * and feed the running watchdog. Set the running watchdog to NULL and set the control to the
 * saved control.
 */
void ClutchControl_DisableSlip(void)
{
#if CONFIG_ENABLE_CLUTCH && CONFIG_ENABLE_CLUTCH_SLIP
	(void)Servo_SetDefaultPos(clutchCtrl.servo);
	GearWatchdog_Feed(clutchCtrl.runningWdg);
#endif
	clutchCtrl.runningWdg = NULL;
	clutchCtrl.control = clutchCtrl.savedCtrl;
}

/**
 * @brief Clutch Control state machine.
 *
 * Controls the clutch servo based on CAN messages received from
 * the steering wheel or gear control module.
 */
static void ClutchControl_StateMachine(void)
{
	switch (clutchCtrl.control) {
		case CLUTCH_CTRL_INIT_CAN_ACK:
			/* Wait for CAN Acknowledge message */
			ClutchControl_CAN_Acknowledge();
			break;

		case CLUTCH_CTRL_NORMAL_OP:
			/* Normal control from steering wheel */
			ClutchControl_CAN_NormalOperation();
			break;

		case CLUTCH_CTRL_GEARCTRL:
			/* Control from gear control module */
			/* It's changed back to normal operation from Gear Control */
			break;

		case CLUTCH_CTRL_CAN_ERROR:
			/* CAN error message */
			(void)Servo_SetDefaultPos(clutchCtrl.servo);
			break;

		case CLUTCH_CTRL_SERVO_DISABLED:
		default:
			/* Clutch disabled due to servo error */
			(void)Servo_Disable(clutchCtrl.servo);
			break;
	}
}

/**
 * @brief Main process function that is called from the Scheduler.
 */
void ClutchControl_Process(void)
{
	ClutchControl_StateMachine();
}

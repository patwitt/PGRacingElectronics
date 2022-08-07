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
typedef struct {
	const uint32_t degMin;
	const uint32_t degDefault;
	const uint32_t degMax;
} ClutchPosLimits;

typedef struct {
	ClutchControlStates control;
	ClutchControlStates savedCtrl;
	GearWatchdogType *runningWdg;
	const ServoTypeEnum servo;
	const CAN_RxMsgEnum canRxMsg;
	bool_t canAck;
} ClutchControlHandler;

static const ServoConfig clutchServoConfig = {
		.limits = {
			 .degMin     = 18U,
		     .degDefault = 18U,
			 .degMax     = 162U
		},
		.pwmChannel = TIM_CHANNEL_1
};

static void ClutchControl_WatchdogElapsedTrigger(void);

static GearWatchdogType upShiftClutchSlipWdg = {
		.elapsedTrigger = ClutchControl_WatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = CLUTCH_UPSHIFT_SLIP_TIMEOUT_MS
};

static GearWatchdogType downShiftClutchSlipWdg = {
		.elapsedTrigger = ClutchControl_WatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = CLUTCH_DOWNSHIFT_SLIP_TIMEOUT_MS
};

static GearWatchdogType *const clutchWdgMap[CLUTCH_DIR_COUNT] = {
	[CLUTCH_DIR_UPSHIFT] = &upShiftClutchSlipWdg,
	[CLUTCH_DIR_DOWNSHIFT] = &downShiftClutchSlipWdg,
};


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
static void ClutchControl_CAN_Acknowledge(void);
static void ClutchControl_CAN_NormalOperation(void);

static void ClutchControl_WatchdogElapsedTrigger(void)
{
	ClutchControl_DisableSlip();
}


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

		err = Servo_Init(clutchCtrl.servo, &clutchServoConfig, clutchServoPwmParams);

		if (err == ERROR_OK) {
			err = GearWatchdog_Init(&upShiftClutchSlipWdg);
		}

		if (err == ERROR_OK) {
			err = GearWatchdog_Init(&downShiftClutchSlipWdg);
		}
	}

	return err;
}

void ClutchControl_TriggerSlip(const uint32_t slipDegrees, const ClutchShiftDirection direction)
{
#if CONFIG_CLUTCH_ENABLE && CONFIG_GEAR_CLUTCH_SLIP_ENABLE
	if (direction < CLUTCH_DIR_COUNT) {
		clutchCtrl.savedCtrl = clutchCtrl.control;
		clutchCtrl.control = CLUTCH_CTRL_GEARCTRL;
		(void)Servo_SetPos(clutchCtrl.servo, slipDegrees);
	}
#endif
	clutchCtrl.runningWdg = clutchWdgMap[direction];
}

void ClutchControl_DisableSlip(void)
{
#if CONFIG_CLUTCH_ENABLE && CONFIG_GEAR_CLUTCH_SLIP_ENABLE
	(void)Servo_SetDefaultPos(clutchCtrl.servo);
	clutchCtrl.control = clutchCtrl.savedCtrl;
	GearWatchdog_Feed(clutchCtrl.runningWdg);
#endif
	clutchCtrl.runningWdg = NULL;
}

void ClutchControl_Process(void)
{
	switch (clutchCtrl.control) {
		case CLUTCH_CTRL_INIT_CAN_ACK:
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
			(void)Servo_SetDefaultPos(clutchCtrl.servo);
			break;

		case CLUTCH_CTRL_SERVO_DISABLED:
		default:
			(void)Servo_Disable(clutchCtrl.servo);
			break;
	}
}

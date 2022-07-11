/*
 * ClutchControl.c
 *
 *  Created on: 11.07.2022
 *      Author: Patryk Wittbrodt
 */

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

static const ServoConfig clutchServoConfig = {
		.limits = {
			 .degMin     = 18U,
		     .degDefault = 18U,
			 .degMax     = 162U
		},
		.pwmChannel = TIM_CHANNEL_1
};

typedef struct {
	ClutchControlStates control;
	const ServoTypeEnum servo;
	const CAN_RxMsgEnum canRxMsg;
	bool_t canAck;
} ClutchControlHandler;

static ClutchControlHandler clutchCtrl = {.control  = CLUTCH_CTRL_INIT_CAN_ACK,
		                                  .servo    = SERVO_CLUTCH,
										  .canRxMsg = CAN_RX_MSG_CLUTCH,
										  .canAck   = FALSE};

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */
static ErrorEnum ClutchControl_EnableServo(void);
static void ClutchControl_GearControl(void);
static void ClutchControl_CAN_Acknowledge(void);
static void ClutchControl_CAN_NormalOperation(void);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static ErrorEnum ClutchControl_EnableServo(void)
{
	ErrorEnum servoErr = Servo_Enable(clutchCtrl.servo);

	if (servoErr == ERROR_OK) {
		servoErr = Servo_SetDefaultPos(clutchCtrl.servo);
	}

	return servoErr;
}

static void ClutchControl_GearControl(void)
{
	/*
	 * @TODO: Control clutch on gear shifts from Gear control module
	 */
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
			if (ClutchControl_EnableServo() == ERROR_OK) {
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
		Servo_SetPos(clutchCtrl.servo, clutchPosFromCan);

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
	}

	return err;
}

void ClutchControl_SetState(const ClutchControlStates clutchControl)
{
	if (clutchControl < CLUTCH_CTRL_COUNT) {
		if (clutchCtrl.control != CLUTCH_CTRL_SERVO_DISABLED) {
			clutchCtrl.control = clutchControl;
		}
	}
}

void ClutchControl_Process(void)
{
	switch (clutchCtrl.control) {
		case CLUTCH_CTRL_INIT_CAN_ACK:
			ClutchControl_CAN_Acknowledge();
			break;

		case CLUTCH_CTRL_NORMAL_OP:
			ClutchControl_CAN_NormalOperation();
			break;

		case CLUTCH_CTRL_GEARCTRL:
			/* Control from gear control module */
			ClutchControl_GearControl();
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

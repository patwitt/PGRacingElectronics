/*
 * GearControlCAN.c
 *
 *  Created on: 30.07.2022
 *      Author: Patryk Wittbrodt
 */

#include "GearControl.h"
#include "GearControlCAN.h"
#include "CAN.h"
#include "SwTimer.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

typedef enum {
	CAN_REPORTING_IN_PROGRESS,
	CAN_REPORTING_IDLE
} CANReportingState;

static const uint32_t canStatusAlivenessMsMap[CAN_SHIFT_DISABLED + 1U] = {
	[CAN_SHIFT_INIT]            = 500U,
	[CAN_SHIFT_IDLE]            = 20U,
	[CAN_SHIFT_EXEC]            = 30U,
	[CAN_SHIFT_FAILURE_ELAPSED] = 200U,
	[CAN_SHIFT_SUCCESS]         = 200U,
	[CAN_GEARSENS_FAILURE]      = 400U,
	[CAN_GEARSENS_UNKNOWN]      = 400U,
	[CAN_GEARSENS_IMPLAUSIBLE]  = 400U,
	[CAN_SHIFT_DISABLED]        = 500U
};

typedef struct {
	SwTimerType timer;
	CANShiftStatus shiftStatus;
	CANShiftStatus newShiftStatus;
	CANReportingState reportingState;
	const uint32_t *const timerMap;
	const CAN_MsgDataBytes gearByte;
	const CAN_MsgDataBytes shiftStatusByte;
	const CAN_TxMsgStdIdEnum msgId;
} CANReportHandler;

static CANReportHandler canReportCtrl = {
		/* Status variables */
		.reportingState = CAN_REPORTING_IDLE,
		.shiftStatus = CAN_SHIFT_INIT,
		/* CAN config */
		.timerMap = canStatusAlivenessMsMap,
		.msgId = CAN_TX_MSG_STDID_GEARINFO,
		.gearByte = CAN_DATA_BYTE_DATA_0,
		.shiftStatusByte = CAN_DATA_BYTE_DATA_1
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline void GearControlCAN_ShiftStatusHandler(void);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static inline void GearControlCAN_ShiftStatusHandler(void)
{
	switch (canReportCtrl.reportingState) {
		/* Reporting in progress */
		case CAN_REPORTING_IN_PROGRESS:
			if (SwTimerHasElapsed(&canReportCtrl.timer)) {
				/* Reporting done */
				canReportCtrl.reportingState = CAN_REPORTING_IDLE;
			} else {
				/* Update shift status */
				CAN_TxUpdateData(canReportCtrl.msgId, canReportCtrl.gearByte, (uint8_t)canReportCtrl.shiftStatus);
			}
			break;

		/* Idle state, ready for new shift status */
		case CAN_REPORTING_IDLE:
			if (canReportCtrl.shiftStatus != canReportCtrl.newShiftStatus) {
				/* Save new shift status */
				canReportCtrl.shiftStatus = canReportCtrl.newShiftStatus;
				/* Start CAN reporting timer for shift status */
				SwTimerStart(&canReportCtrl.timer, canReportCtrl.timerMap[canReportCtrl.shiftStatus]);
				/* Go to reporting in progress */
				canReportCtrl.reportingState = CAN_REPORTING_IN_PROGRESS;
			}
			break;

		default:
			/* Do nothing */
			break;
	}
}
/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum GearControlCAN_Init(void)
{
	ErrorEnum err = ERROR_OK;

	err = SwTimerRegister(&canReportCtrl.timer);

	return err;
}

void GearControlCAN_UpdateStatus(const CANShiftStatus shiftStatus)
{
	/* New shift status will be processed only in IDLE reporting state */
	canReportCtrl.newShiftStatus = shiftStatus;
}

void GearControlCAN_Process(void)
{
	/* Update gear data */
	CAN_TxUpdateData(canReportCtrl.msgId, canReportCtrl.gearByte, (uint8_t)GearControl_GetGear());

	/* Update shift status data */
	GearControlCAN_ShiftStatusHandler();
}

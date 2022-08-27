/*
 * GearControlCAN.c
 *
 *  Created on: 30.07.2022
 *      Author: Patryk Wittbrodt
 */

#include "GearControlCAN.h"
#include "DefineConfig.h"
#if CONFIG_ENABLE_CAN
#include "GearControl.h"
#include "CAN.h"
#include "SwTimer.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

//! CAN reporting states
typedef enum {
	CAN_REPORTING_IN_PROGRESS,
	CAN_REPORTING_IDLE
} CANReportingState;

//! Mapping of CAN signal <- time of reporting in ms
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

//! CAN reporting handler struct
typedef struct {
	SwTimerType timer;
	CANShiftStatus shiftStatus;
	CANShiftStatus newShiftStatus;
	CANReportingState reportingState;
	const uint32_t *const timerMap;
	const CAN_MsgDataBytes gearByte;
	const CAN_MsgDataBytes shiftStatusByte;
	const CAN_TxMsgStdIdEnum msgId;
	SwTimerStats *gearTimStats;
} CANReportHandler;

//! CAN Reporting handler
static CANReportHandler canReportCtrl = {
		/* Status variables */
		.reportingState = CAN_REPORTING_IDLE,
		.shiftStatus = CAN_SHIFT_INIT,
		/* CAN config */
		.timerMap = canStatusAlivenessMsMap,
		.msgId = CAN_TX_MSG_STDID_GEARINFO,
		.gearByte = CAN_DATA_BYTE_0,
		.shiftStatusByte = CAN_DATA_BYTE_1,
		.gearTimStats = NULL
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline void GearControlCAN_ShiftStatusHandler(void);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */
/**
 * @brief CAN Shift status handler.
 * 
 * If the reporting state is idle, and the new shift status is different from the current shift status,
 * then save the new shift status, start the timer, and set the reporting state to in progress. If the
 * reporting state is in progress, and the timer has elapsed, then set the reporting state to idle. If
 * the reporting state is in progress, and the timer has not elapsed, then update the shift status.
 */
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
				CAN_TxUpdateData(CAN_TX_MSG_GEARINFO, canReportCtrl.gearByte, (uint8_t)GearControl_GetGear());
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
/**
 * @brief Initialization of Gear Control CAN module.
 * 
 * @return an error code.
 */
ErrorEnum GearControlCAN_Init(SwTimerStats *const gearTimStats)
{
	ErrorEnum err = ERROR_OK;

	err = SwTimerRegister(&canReportCtrl.timer);

	if (gearTimStats != NULL) {
		canReportCtrl.gearTimStats = gearTimStats;
	} else {
		err = ERROR_NULL;
	}

	return err;
}

/**
 * @brief Update shift status.
 *
 * If the current reporting state is IDLE, then the new shift status will be processed.
 * 
 * @param shiftStatus The current shift status.
 */
void GearControlCAN_UpdateStatus(const CANShiftStatus shiftStatus)
{
	/* New shift status will be processed only in IDLE reporting state */
	canReportCtrl.newShiftStatus = shiftStatus;
}

/**
 * @brief Main process function that is called from the Scheduler.
 */
void GearControlCAN_Process(void)
{
	/* Update gear data */
	CAN_TxUpdateData(CAN_TX_MSG_GEARINFO, canReportCtrl.gearByte, (uint8_t)GearControl_GetGear());

	/* Update shift status data */
	GearControlCAN_ShiftStatusHandler();
}
#else
ErrorEnum GearControlCAN_Init(void) { return ERROR_OK; }
void GearControlCAN_UpdateStatus(const CANShiftStatus shiftStatus) { (void)shiftStatus; }
void GearControlCAN_Process(void) {}
#endif

/*
 * GearControlCAN.h
 *
 *  Created on: 30.07.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_GEARCONTROL_CAN_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_GEARCONTROL_CAN_H_

#include "Types.h"

typedef enum {
	CAN_SHIFT_INIT                = 0x00U,
	CAN_SHIFT_IDLE                = 0x01U,
	CAN_SHIFT_EXEC                = 0x02U,
	CAN_SHIFT_FAILURE_WDG_ELAPSED = 0x03U,
	CAN_SHIFT_SUCCESS             = 0x04U,
	CAN_GEARSENS_UNKNOWN          = 0x05U,
	CAN_GEARSENS_FAILURE          = 0x06U,
	CAN_GEARSENS_IMPLAUSIBLE      = 0x07U,
	CAN_SHIFT_DISABLED            = 0x08U,
	CAN_SHIFT_STATUS_COUNT        = 0x09U
} CANShiftStatus;

ErrorEnum GearControlCAN_Init(void);
void GearControlCAN_Process(void);
void GearControlCAN_UpdateStatus(const CANShiftStatus shiftStatus);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARCONTROL_CAN_H_ */

/*
 * ShiftControl.h
 *
 *  Created on: 28.08.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_SHIFTCONTROL_INC_GEARCONTROL_H_
#define SRC_APPLICATION_SHIFTCONTROL_INC_GEARCONTROL_H_

#include "Types.h"
#include "stm32f4xx_hal.h"
#include "SwTimer.h"

#define GEAR_SHIFT_TIMEOUT_MS (300U)

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
} ShiftStates;

ErrorEnum ShiftControl_Init(__IO SwTimerStats *const gearTimStats);
GearStates ShiftControl_Handler(void);
ShiftStates ShiftControl_GetState(void);

#endif /* SRC_APPLICATION_SHIFTCONTROL_INC_GEARCONTROL_H_ */

/*
 * MicroSwitch.h
 *
 *  Created on: 24.04.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_MIDDLEWARE_DRIVERS_INC_MICROSWITCH_H_
#define SRC_MIDDLEWARE_DRIVERS_INC_MICROSWITCH_H_

#include "Types.h"

typedef enum {
	MS_G_DOWN = 0U,
	MS_G_UP   = 1U,
	MS_COUNT  = 2U
} MicroSwitchTypeEnum;

typedef enum {
	MS_STATE_LOW        = 0U,
	MS_STATE_HIGH       = 1U,
	MS_STATE_DEBOUNCING = 2U,
	MS_STATE_COUNT      = 3U
} MicroSwitchStateEnum;

typedef enum {
	MS_CONTROL_ENABLED,
	MS_CONTROL_DISABLED,
	MS_CONTROL_DEBOUNCE_LOW
} MicroSwitchControlType;

typedef struct {
	__IO uint32 debCnt;
	__IO uint32 validCnt;
	const uint32 GPIO;
} MicroSwitchInternal;

typedef struct {
	MicroSwitchInternal *const internal;
	__IO MicroSwitchStateEnum state;
} MicroSwitch;

void MicroSwitch_Init(void);
void MicroSwitch_Process(void);
void MicroSwitch_SetControl(MicroSwitchControlType control);
MicroSwitchControlType MicroSwitch_GetControl(void);
__IO MicroSwitch* MicroSwitch_Get(MicroSwitchTypeEnum microswitch);

#endif /* SRC_MIDDLEWARE_DRIVERS_INC_MICROSWITCH_H_ */

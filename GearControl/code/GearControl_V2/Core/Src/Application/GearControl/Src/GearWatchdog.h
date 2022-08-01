/*
 * GearWatchdog.h
 *
 *  Created on: 20.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_GEARWATCHDOG_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_GEARWATCHDOG_H_

#include "Types.h"
#include "SwTimer.h"

typedef enum {
	GEAR_WATCHDOG_STATUS_FED_ON_TIME,
	GEAR_WATCHDOG_STATUS_ELAPSED,
	GEAR_WATCHDOG_STATUS_ACTIVE,
	GEAR_WATCHDOG_STATUS_INACTIVE,
	GEAR_WATCHDOG_STATUS_COUNT
} GearWatchdogStatusEnum;

typedef enum {
	GEAR_WDG_SHIFT      = 0U,
	GEAR_WDG_FUEL_CUT   = 1U,
	GEAR_WDG_COUNT      = 2U
} GearWatchdogTypeEnum;

typedef void (*WatchdogFailTrigger)(void);

typedef struct {
	__IO GearWatchdogStatusEnum status;
	WatchdogFailTrigger elapsedTrigger;
	SwTimerType timer;
	const uint32_t timeoutMs;
} GearWatchdogType;

void GearWatchdog_Process(void);

boolean GearWatchdog_HasElapsed(GearWatchdogType *const wdgEntity);
ErrorEnum GearWatchdog_Init(GearWatchdogType *const wdgEntity);
void GearWatchdog_Start(GearWatchdogType *const wdgEntity);
void GearWatchdog_Feed(GearWatchdogType *const wdgEntity);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARWATCHDOG_H_ */

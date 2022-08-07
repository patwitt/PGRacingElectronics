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
	GEAR_WDG_SHIFT            = 0U,
	GEAR_WDG_INJECTORS_CUT    = 1U,
	GEAR_WDG_UPSHIFT_CLUTCH   = 2U,
	GEAR_WDG_DOWNSHIFT_CLUTCH = 3U,
	GEAR_WDG_COUNT            = 4U
} GearWatchdogTypeEnum;

typedef void (*WatchdogElapsedTrigger)(void);

typedef struct {
	__IO GearWatchdogStatusEnum status;
	WatchdogElapsedTrigger elapsedTrigger;
	SwTimerType timer;
	const uint32_t timeoutMs;
} GearWatchdogType;

void GearWatchdog_Process(void);
ErrorEnum GearWatchdog_Init(GearWatchdogType *const wdgEntity);
void GearWatchdog_Start(GearWatchdogType *const wdgEntity);
void GearWatchdog_Feed(GearWatchdogType *const wdgEntity);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARWATCHDOG_H_ */

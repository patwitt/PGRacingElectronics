/*
 * GearWatchdog.h
 *
 *  Created on: 20.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_GEARWATCHDOG_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_GEARWATCHDOG_H_

#include "Types.h"

typedef enum {
	GEAR_WDG_SHIFT      = 0U,
	GEAR_WDG_FUEL_CUT   = 1U,
	GEAR_WDG_COUNT      = 2U
} GearWatchdogTypeEnum;

typedef enum {
	GEAR_WATCHDOG_STATUS_FED_ON_TIME,
	GEAR_WATCHDOG_STATUS_ELAPSED,
	GEAR_WATCHDOG_STATUS_ACTIVE,
	GEAR_WATCHDOG_STATUS_INACTIVE,
	GEAR_WATCHDOG_STATUS_COUNT
} GearWatchdogStatusEnum;

ErrorEnum GearWatchdog_Init(void);
void GearWatchdog_Process(void);
void GearWatchdog_Start(const GearWatchdogTypeEnum watchdog);
void GearWatchdog_Feed(const GearWatchdogTypeEnum watchdog);
GearWatchdogStatusEnum GearWatchdog_GetStatus(const GearWatchdogTypeEnum watchdog);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARWATCHDOG_H_ */

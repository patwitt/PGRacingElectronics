/*
 * GearWatchdog.c
 *
 *  Created on: 19.06.2022
 *      Author: Patryk Wittbrodt
 */
#include "GearWatchdog.h"
#include "SwTimer.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
typedef struct {
	__IO GearWatchdogStatusEnum status;
	SwTimerType timer;
	const uint32_t timeoutMs;
} GearWatchdogType;

static GearWatchdogType gearWdg[GEAR_WDG_COUNT] = {{.status = GEAR_WATCHDOG_STATUS_INACTIVE, .timeoutMs = 200U},  // Shift time
												   {.status = GEAR_WATCHDOG_STATUS_INACTIVE, .timeoutMs = 100U}   // Fuel cut
};

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum GearWatchdog_Init(void)
{
	ErrorEnum err = ERROR_OK;

	err = SwTimerInit(&gearWdg[GEAR_WDG_SHIFT].timer, gearWdg[GEAR_WDG_SHIFT].timeoutMs, FALSE);
	if (err == ERROR_OK) {
		err = SwTimerRegister(&gearWdg[GEAR_WDG_SHIFT].timer);
	}

	if (err == ERROR_OK) {
		err = SwTimerInit(&gearWdg[GEAR_WDG_FUEL_CUT].timer, gearWdg[GEAR_WDG_FUEL_CUT].timeoutMs, FALSE);

		if (err == ERROR_OK) {
			err = SwTimerRegister(&gearWdg[GEAR_WDG_FUEL_CUT].timer);
		}
	}

	return err;
}

void GearWatchdog_Process(void)
{
	for (uint32_t i = 0U; i < GEAR_WDG_COUNT; ++i) {
		GearWatchdogType *const watchdog = &gearWdg[i];

		switch (watchdog->status) {
			case GEAR_WATCHDOG_STATUS_ACTIVE:
				if (SwTimerHasTimerElapsed(&watchdog->timer)) {
					/* Watchdog exceeded */
					watchdog->status = GEAR_WATCHDOG_STATUS_ELAPSED;
				}
				break;

			default:
				break;
		}
	}
}

void GearWatchdog_Start(const GearWatchdogTypeEnum watchdog)
{
	if (watchdog < GEAR_WDG_COUNT) {
		SwTimerStart(&gearWdg[watchdog].timer);
		gearWdg[watchdog].status = GEAR_WATCHDOG_STATUS_ACTIVE;
	}
}

void GearWatchdog_Feed(const GearWatchdogTypeEnum watchdog)
{
	if (watchdog < GEAR_WDG_COUNT) {
		gearWdg[watchdog].status = GEAR_WATCHDOG_STATUS_FED_ON_TIME;
	}
}

GearWatchdogStatusEnum GearWatchdog_GetStatus(const GearWatchdogTypeEnum watchdog)
{
	return (watchdog < GEAR_WDG_COUNT) ? gearWdg[watchdog].status : GEAR_WATCHDOG_STATUS_COUNT;
}


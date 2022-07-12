/*
 * GearWatchdog.c
 *
 *  Created on: 19.06.2022
 *      Author: Patryk Wittbrodt
 */
#include "GearWatchdog.h"
#include "Utils.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

static GearWatchdogType* gearWdg[GEAR_WDG_COUNT] = {NULL, NULL};

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum GearWatchdog_Init(GearWatchdogType *const wdgEntity)
{
	ErrorEnum err = ERROR_OK;
	static uint32_t wdgCount = 0U;

	if (wdgCount < GEAR_WDG_COUNT) {
		err = SwTimerInit(&wdgEntity->timer, wdgEntity->timeoutMs, FALSE);
		if (err == ERROR_OK) {
			err = SwTimerRegister(&wdgEntity->timer);
		}

		if (err == ERROR_OK) {
			/* Register Watchdog Entity */
			gearWdg[wdgCount] = wdgEntity;
			++wdgCount;
		}
	} else {
		err = ERROR_GEAR_WATCHDOG_INIT;
	}

	return err;
}

void GearWatchdog_Process(void)
{
	for (uint32_t i = 0U; i < GEAR_WDG_COUNT; ++i) {
		GearWatchdogType *const watchdog = gearWdg[i];

		if (NULL_CHECK1(watchdog)) {
			switch (watchdog->status) {
				case GEAR_WATCHDOG_STATUS_ACTIVE:
					if (SwTimerHasTimerElapsed(&watchdog->timer)) {
						/* Watchdog exceeded */
						watchdog->elapsedTrigger();
						watchdog->status = GEAR_WATCHDOG_STATUS_ELAPSED;
					}
					break;

				default:
					break;
			}
		}
	}
}

void GearWatchdog_Start(GearWatchdogType *const wdgEntity)
{
	if (NULL_CHECK1(wdgEntity)) {
		SwTimerStart(&wdgEntity->timer);
		wdgEntity->status = GEAR_WATCHDOG_STATUS_ACTIVE;
	}
}

void GearWatchdog_Feed(GearWatchdogType *const wdgEntity)
{
	if (NULL_CHECK1(wdgEntity)) {
		 if (wdgEntity->status == GEAR_WATCHDOG_STATUS_ACTIVE) {
			 wdgEntity->status = GEAR_WATCHDOG_STATUS_FED_ON_TIME;
		}
	}
}

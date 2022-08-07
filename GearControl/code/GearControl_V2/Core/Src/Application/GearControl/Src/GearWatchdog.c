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

//! An array of pointers to all watchdog entities.
static GearWatchdogType* gearWdg[GEAR_WDG_COUNT] = {
	[GEAR_WDG_SHIFT]            = NULL,
	[GEAR_WDG_INJECTORS_CUT]    = NULL,
	[GEAR_WDG_UPSHIFT_CLUTCH]   = NULL,
	[GEAR_WDG_DOWNSHIFT_CLUTCH] = NULL
};

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
/**
 * @brief Initialization of the Gear Watchdog module.
 *
 * The function initializes a watchdog entity and registers it in the global array of watchdog
 * entities.
 * 
 * @param wdgEntity This is a pointer to the watchdog entity.
 * 
 * @return an error code.
 */
ErrorEnum GearWatchdog_Init(GearWatchdogType *const wdgEntity)
{
	ErrorEnum err = ERROR_OK;
	static uint32_t wdgCount = 0U;

	if (wdgCount < GEAR_WDG_COUNT) {
		err = SwTimerRegister(&wdgEntity->timer);

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

/**
 * @brief Main process function that is called from the Scheduler.
 * 
 * If the watchdog is active and the timer has elapsed, call the elapsedTrigger function and set the
 * status to elapsed.
 */
void GearWatchdog_Process(void)
{
	for (uint32_t i = 0U; i < GEAR_WDG_COUNT; ++i) {
		GearWatchdogType *const watchdog = gearWdg[i];

		if (NULL_CHECK1(watchdog)) {
			switch (watchdog->status) {
				case GEAR_WATCHDOG_STATUS_ACTIVE:
					if (SwTimerHasElapsed(&watchdog->timer)) {
						/* Watchdog exceeded */
						watchdog->elapsedTrigger();
						watchdog->status = GEAR_WATCHDOG_STATUS_ELAPSED;
					}
					break;

				case GEAR_WATCHDOG_STATUS_FED_ON_TIME:
				case GEAR_WATCHDOG_STATUS_ELAPSED:
				default:
					/* Do nothing in any other state */
					break;
			}
		}
	}
}

/**
 * @brief Start the watchdog timer.
 * 
 * @param wdgEntity A pointer to the watchdog entity.
 */
void GearWatchdog_Start(GearWatchdogType *const wdgEntity)
{
	if (NULL_CHECK1(wdgEntity)) {
		SwTimerStart(&wdgEntity->timer, wdgEntity->timeoutMs);
		wdgEntity->status = GEAR_WATCHDOG_STATUS_ACTIVE;
	}
}

/**
 * @brief Feed the watchdog.
 * 
 * If the watchdog is active, then mark it as fed on time.
 * 
 * @param wdgEntity Pointer to the watchdog entity.
 */
void GearWatchdog_Feed(GearWatchdogType *const wdgEntity)
{
	if (NULL_CHECK1(wdgEntity)) {
		 if (wdgEntity->status == GEAR_WATCHDOG_STATUS_ACTIVE) {
			 wdgEntity->status = GEAR_WATCHDOG_STATUS_FED_ON_TIME;
		}
	}
}

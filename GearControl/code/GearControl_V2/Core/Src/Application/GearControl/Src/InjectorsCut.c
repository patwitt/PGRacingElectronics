/*
 * InjectorsCut.c
 *
 *  Created on: 12.07.2022
 *      Author: Patryk Wittbrodt
 */
#include "InjectorsCut.h"
#include "GearWatchdog.h"

/* ---------------------------- */
/*         Local data           */
/* ---------------------------- */

static void InjectorsCutWatchdogElapsedTrigger(void);

static GearWatchdogType injectorsCutWdg = {
		.elapsedTrigger = InjectorsCutWatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = 100U
};

typedef struct {
	GearWatchdogType *const watchdog;
	const uint32 GPIO;
} InjectorsCutHandler;

static __IO InjectorsCutHandler injectorsHandler = {.watchdog = &injectorsCutWdg, .GPIO = 0U};

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
void InjectorsCutWatchdogElapsedTrigger(void)
{
	/* Disable Injectors Cut */

}

ErrorEnum InjectorsCut_Init(void)
{
	ErrorEnum err = ERROR_OK;

	/* Initialize Injectors Cut Watchdog */
	if (err == ERROR_OK) {
		err = GearWatchdog_Init(injectorsHandler.watchdog);
	}

	return err;
}

void InjectorsCut_Trigger(void)
{
	/* Start Watchdog */
	GearWatchdog_Start(injectorsHandler.watchdog);

	/* Trigger injectors cut */
}

void InjectorsCut_Finish(void)
{
	GearWatchdog_Feed(injectorsHandler.watchdog);
}

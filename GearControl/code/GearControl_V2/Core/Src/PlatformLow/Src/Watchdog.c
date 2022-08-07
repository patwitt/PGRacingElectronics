/*
 * Watchdog.c
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "Watchdog.h"
#include "DefineConfig.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
static __IO IWDG_HandleTypeDef* watchdog_ = NULL;

void Watchdog_Init(__IO IWDG_HandleTypeDef* const wdg)
{
	watchdog_ = wdg;
}

__IO void WatchdogFeed(void)
{
#if CONFIG_ENABLE_SCHEDULER_WATCHDOG
	__HAL_IWDG_RELOAD_COUNTER(watchdog_);
#endif
}

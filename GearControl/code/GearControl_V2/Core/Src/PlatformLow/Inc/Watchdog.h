/*
 * Watchdog.h
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_PLATFORMLOW_SRC_WATCHDOG_H_
#define SRC_PLATFORMLOW_SRC_WATCHDOG_H_

#include "Types.h"
#include "stm32f4xx_hal.h"

void Watchdog_Init(__IO IWDG_HandleTypeDef* const wdg);
__IO void WatchdogFeed(void);

#endif /* SRC_PLATFORMLOW_SRC_WATCHDOG_H_ */

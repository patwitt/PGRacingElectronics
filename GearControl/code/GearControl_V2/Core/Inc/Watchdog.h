/*
 * Watchdog.h
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_WATCHDOG_H_
#define INC_WATCHDOG_H_

#include "main.h"

void Watchdog_Init(__IO IWDG_HandleTypeDef* const wdg);
__IO void WatchdogFeed(void);

#endif /* INC_WATCHDOG_H_ */

/*
 * PID.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_LED_CTRL_H_
#define INC_LED_CTRL_H_

#include "Types.h"
#include "DefineConfig.h"

typedef enum {
	LED_OFF,
	LED_BLINK_1HZ,
	LED_SOLID
} LEDStatus;

void LED_Process(void);
void LED_SetStatus(LEDStatus status);
void LED_indicateResetWithDelay(const uint32_t periodx100ms);
#endif /* INC_LED_CTRL_H_ */

/*
 * LED.c
 *
 *  Created on: 18.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "LED.h"

#include "Types.h"
#include "stm32f4xx_hal.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
static LEDStatus ledStatus = LED_OFF;

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
void LED_Process(void) {
	switch (ledStatus) {
		case LED_OFF:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
			break;

		case LED_BLINK_1HZ:
			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_SET) {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
			} else {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
			}
			break;

		case LED_SOLID:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
			break;

		default:
			break;
	}
}

void LED_SetStatus(LEDStatus newStatus) {
	ledStatus = newStatus;
}

/*
 * LED.c
 *
 *  Created on: 18.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "LED.h"

#include "Types.h"
#include "stm32f4xx_hal.h"
#include "Watchdog.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
#define DELAY_100MS (100U)

static LEDStatus ledStatus = LED_OFF;
static void LED_Delay(const uint32_t Delay);

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
void LED_Process(void)
{
	switch (ledStatus) {
		case LED_OFF:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
			break;

		case LED_BLINK_1HZ:
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
			break;

		case LED_SOLID:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
			break;

		default:
			break;
	}
}

void LED_SetStatus(LEDStatus newStatus)
{
	ledStatus = newStatus;
}

void LED_indicateResetWithDelay(const uint32_t periodx100ms)
{
	for (uint32_t i = 0U; i < periodx100ms; ++i) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
		LED_Delay(DELAY_100MS);
	}
}

static void LED_Delay(const uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY) {
    wait += (uint32_t)(uwTickFreq);
  }

  uint32 tick = HAL_GetTick();
  uint32_t prevTick = 0U;

  while((tick - tickstart) < wait)
  {
	  tick = HAL_GetTick();

	  if (tick != prevTick) {
		  /* Keep watchdog alive */
		  WatchdogFeed();
	  }
	  prevTick = tick;
  }
}

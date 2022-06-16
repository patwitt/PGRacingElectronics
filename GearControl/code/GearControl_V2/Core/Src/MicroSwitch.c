/*
 * MicroSwitch.c
 *
 *  Created on: 24.04.2021
 *      Author: Patryk Wittbrodt
 */

#include "GearControl.h"
#include "main.h"
#include "MicroSwitch.h"

typedef struct
{
	__IO uint16 debCnt;
	__IO uint16 validCnt;
	__IO uint8 request;
	__IO boolean wasPressed;
	__IO uint16 GPIO;
	__IO uint16* adc_read;
}Button;

static __IO uint8 lowDebCnt = 0;
static __IO boolean CheckForLow = TRUE;
static __IO Button MicroSwitch[BUTTON_CNT];
static __IO uint8 loop_end = 0;
static __IO PollStates MS_PollStatus = POLL_NONE;

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */
static void MicroSwitch_Clear(void);
static void MicroSwitch_DebounceLow(void);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static void MicroSwitch_Clear(void)
{
	uint8 i = 0;

	for (i = 0; i < BUTTON_CNT; ++i)
	{
		MicroSwitch[i].validCnt = 0;
		MicroSwitch[i].debCnt = 0;
		MicroSwitch[i].wasPressed = FALSE;
	}
}

static void MicroSwitch_DebounceLow(void)
{
	/* Make sure both microswitches were LOW debouncing before triggering gearshift again */
	if ((*MicroSwitch[0].adc_read < BUTTON_OFF_ADC_TRESHOLD) &&
		(*MicroSwitch[1].adc_read < BUTTON_OFF_ADC_TRESHOLD))
	{
		++lowDebCnt;

		if (lowDebCnt > DEBOUNCE_20MS)
		{
			lowDebCnt = 0;
			CheckForLow = FALSE;
		}
	}
	else
	{
		lowDebCnt = 0;
	}
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

void MicroSwitch_Init(__IO uint16* const dmaBuffer)
{
	uint8 i = 0;

	for (i = 0; i < BUTTON_CNT; ++i)
	{
		MicroSwitch[i].debCnt = 0;
		MicroSwitch[i].validCnt = 0;
		MicroSwitch[i].wasPressed = FALSE;
	}

	MicroSwitch[0].request = REQUEST_SHIFT_DOWN;
	MicroSwitch[0].GPIO = GPIO_PIN_15;
	MicroSwitch[0].adc_read = &dmaBuffer[2];
	MicroSwitch[1].request = REQUEST_SHIFT_UP;
	MicroSwitch[1].GPIO = GPIO_PIN_14;
	MicroSwitch[1].adc_read = &dmaBuffer[3];
}

void MicroSwitch_PollAgain(void)
{
	MS_PollStatus = POLL_NONE;
}

void MicroSwitch_PollStatus(void)
{
	if ((SHIFT_IDLE == ShiftControl_GetState()) && (MS_PollStatus == POLL_NONE))
	{
		if (CheckForLow)
		{
			MicroSwitch_DebounceLow();
		}
		else
		{
			if (*MicroSwitch[0].adc_read > BUTTON_PRESSED_ADC_THRESHOLD)
			{
				MS_PollStatus = POLL_SHIFT_DOWN;
			}
			else if (*MicroSwitch[1].adc_read > BUTTON_PRESSED_ADC_THRESHOLD)
			{
				MS_PollStatus = POLL_SHIFT_UP;
			}
			else
			{
				/* Nothing */
			}
		}
	}
}

void OnOffSwitch_Process(void)
{
	static uint8_t onOffCnt = 0U;
	static uint8_t debCnt = 0U;
	static const uint8_t DEB_THRESHOLD = 4U; // * 200ms

	++debCnt;

	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) // low pin
	{
		++onOffCnt;
	}
	else // low pin
	{
		onOffCnt = 0U;
	}

	if (debCnt > DEB_THRESHOLD)
	{
		if (onOffCnt == debCnt)
		{
			GearControl_SetControl(GEAR_DISABLE);
		}
		else
		{
			GearControl_SetControl(GEAR_ENABLE);
		}

		onOffCnt = 0U;
		debCnt = 0U;
	}
}

void MicroSwitch_Process(void)
{
	if ((MS_PollStatus == POLL_SHIFT_UP) || (MS_PollStatus == POLL_SHIFT_DOWN))
	{
		uint8 id = (uint8)MS_PollStatus;

		if (MicroSwitch[id].debCnt <= DEBOUNCE_10MS)
		{
			++MicroSwitch[id].debCnt;

			if (*MicroSwitch[id].adc_read > BUTTON_PRESSED_ADC_THRESHOLD)
			{
				++MicroSwitch[id].validCnt;
			}
			else
			{
				MicroSwitch[id].validCnt = 0U;
			}

			/* Check for X CONSECUTIVE high signals */
			if (MicroSwitch[id].validCnt >= DEBOUNCE_10MS)
			{
				MicroSwitch[id].wasPressed = TRUE;
			}
		}
		else
		{
			/* Debounce exceeded */
			MicroSwitch_Clear();
			MicroSwitch_PollAgain();
		}

		/* Microswitch pressed, change gear */
		if (TRUE == MicroSwitch[id].wasPressed)
		{
			CheckForLow = TRUE;
			MS_PollStatus = POLL_EXEC;
			MicroSwitch_Clear();
			ShiftControl_SetRequest(MicroSwitch[id].request);
		}
	}
}

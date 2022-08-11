#include "tim.h"
#include "math.h"
#include <stdbool.h>
#include "WS2812_driver.h"

//extern TIM_HandleTypeDef htim3;
//extern DMA_HandleTypeDef hdma_tim3_ch1_trig;

#define BIT_0_TIME		43
#define BIT_1_TIME		86

#define RESET_LEN		40
#define LED_N			15

volatile uint16_t leds[RESET_LEN + 24 * LED_N];

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
}

void WS2812_Init(void)
{
	for (int i = 0; i < 24 * LED_N; i++)
		leds[i] = BIT_0_TIME;

	for (int i = 0; i < RESET_LEN; i++)
		leds[i + LED_N * 24] = 0;

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*) leds,	RESET_LEN + 24 * LED_N);
}

void setLed(uint8_t ledNumber, uint8_t red, uint8_t green, uint8_t blue)
{
	const uint8_t lookUpTable[8] = {128, 64, 32, 16, 8, 4, 2, 1};
	uint16_t ledId = 24 * ledNumber;
	for (int i = 0; i < 8; i++)
	{
		leds[ledId] = red & 128 / lookUpTable[i] ? BIT_1_TIME : BIT_0_TIME;
		leds[ledId+8] = green & 128 / lookUpTable[i] ? BIT_1_TIME : BIT_0_TIME;
		leds[ledId+16] = blue & 128 /lookUpTable[i] ? BIT_1_TIME : BIT_0_TIME;
		ledId++;
	}
}

void updateLeds(int rpm, int mode)
{
	uint16_t minRPM = 8000;
	uint16_t maxRPM = 11500;
	if(mode == 1)
	{
		minRPM = 0;
		maxRPM = 12000;
	}
	else
	{
		minRPM = 8500;
		maxRPM = 11500;
	}

	int8_t activeLeds = ceil((rpm - minRPM) / (maxRPM - minRPM) * LED_N);
	for (int i = 0; i < LED_N - 5; i++)
	{
		if (i < activeLeds)
		{
			if (i < 5)
			{
				setLed(i, 64, 0, 0);
			}
			else if (i < 10)
			{
				setLed(i, 0, 64, 0);
			}
			else
			{
				if (mode == 3)
				{
					for (int j = i; j < LED_N; j++)
					{
						setLed(j, 0, 0, 64);
					}
				}
				else
				{
					setLed(i,0,0,64);
				}
			}
		}
	}

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, RESET_LEN + 24 * LED_N);
}


void calculateOptimalShiftPoints()
{

}



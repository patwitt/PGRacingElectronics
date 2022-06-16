/*
 * StopWatch.c
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "main.h"
#include "StopWatch.h"
#include "stm32f4xx_hal_tim.h"

static TIM_HandleTypeDef* tim = NULL;

void StopWatchInit(TIM_HandleTypeDef* const timer)
{
	tim = timer;
}

void TimerStopWatchStartFrame(TimerStopWatchLapType* frameTimer)
{
   __HAL_TIM_SET_COUNTER(tim, 0U);

   frameTimer->begin = 0U;
}

void TimerStopWatchStartLap(TimerStopWatchLapType* lap)
{
   lap->begin = __HAL_TIM_GET_COUNTER(tim);
}

uint32_t TimerStopWatchCaptureDuration(TimerStopWatchLapType* lap)
{
   uint32_t retVal = 0U;

   lap->finish = __HAL_TIM_GET_COUNTER(tim);
   retVal = (lap->finish > lap->begin) ? (lap->finish - lap->begin) : 0U;

   return retVal;
}

/*
 * StopWatch.c
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "StopWatch.h"
#include "Types.h"

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

uint32 TimerStopWatchCaptureDuration(TimerStopWatchLapType* lap)
{
   uint32 retVal = 0U;

   lap->finish = __HAL_TIM_GET_COUNTER(tim);
   retVal = (lap->finish > lap->begin) ? (lap->finish - lap->begin) : 0U;

   return retVal;
}

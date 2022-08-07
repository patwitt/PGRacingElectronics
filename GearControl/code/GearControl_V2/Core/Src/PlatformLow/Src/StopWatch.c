/*
 * StopWatch.c
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "StopWatch.h"
#include "Types.h"

static TIM_HandleTypeDef* tim = NULL;

/**
 * @brief Initialization of the StopWatch module.
 * 
 * @param timer The timer to use for the StopWatch.
 */
void StopWatchInit(TIM_HandleTypeDef* const timer)
{
	tim = timer;
}

/**
 * @brief Timer StopWatch start frame.
 * 
 * Set the counter to zero and set the begin value to zero.
 * Used by Scheduler to measure time of certain frame/task.
 * 
 * @param frameTimer pointer to a TimerStopWatchLapType structure.
 */
void TimerStopWatchStartFrame(TimerStopWatchLapType* frameTimer)
{
   __HAL_TIM_SET_COUNTER(tim, 0U);

   frameTimer->begin = 0U;
}

/**
 * @brief Start StopWatch lap.
 * 
 * The function takes a pointer to a TimerStopWatchLapType structure, and sets the begin member of
 * that structure to the current value of the timer's counter register.
 * 
 * @param lap Pointer to a TimerStopWatchLapType structure.
 */
void TimerStopWatchStartLap(TimerStopWatchLapType* lap)
{
   lap->begin = __HAL_TIM_GET_COUNTER(tim);
}

/**
 * @brief Capture duration of the lap.
 * 
 * The function returns the duration of the lap by subtracting the lap's begin value from the lap's
 * finish value.
 * 
 * @param lap pointer to a TimerStopWatchLapType structure
 * 
 * @return The duration of the lap.
 */
uint32 TimerStopWatchCaptureDuration(TimerStopWatchLapType* lap)
{
   uint32 retVal = 0U;

   lap->finish = __HAL_TIM_GET_COUNTER(tim);
   retVal = (lap->finish > lap->begin) ? (lap->finish - lap->begin) : 0U;

   return retVal;
}

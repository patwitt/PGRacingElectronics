#ifndef HAL_STOPWATCH_H
#define HAL_STOPWATCH_H

#include "main.h"

/**
 * Structure for tracking time between events within a frame.
 */
typedef struct
{
   uint32 begin; /**< The time the event began according to the hardware timer */
   /**
    * The last captured time of finish from the hardware timer.
    * This field exists to prevent stack allocations of an extra uint every time the HW timer capture is called.
    */
   uint32 finish; /**< */
} TimerStopWatchLapType;

void StopWatchInit(TIM_HandleTypeDef* const timer);

void TimerStopWatchStartFrame(TimerStopWatchLapType* frameTimer);

void TimerStopWatchStartLap(TimerStopWatchLapType* lap);

uint32 TimerStopWatchCaptureDuration(TimerStopWatchLapType* lap);

#endif /* HAL_STOPWATCH_H */

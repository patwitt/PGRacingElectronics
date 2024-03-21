/*
 * Scheduler.c
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "Scheduler.h"

#include "main.h"
#include "StopWatch.h"
#include "Watchdog.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

static __IO uint32 timers[N_PROCESS];
static __IO boolean hasElapsed = FALSE;

static SwTimerStats combinedStats_; /**< Single frame statistics */
static SchedulerType* schedule_ = NULL;  /**< Pointer to list of tasks */

static void SchedulerExecuteTasks(void);
static void SchedulerTimeBase(void);

/**
 * @brief Initialization of the Scheduler module.
 *
 * Initialize the scheduler and start the timer.
 * 
 * @param schedule pointer to the array of SchedulerType structures.
 * @param timer    pointer to the timer used for the scheduler.
 * 
 * @return an error code.
 */
ErrorEnum SchedulerInit(SchedulerType* const schedule, TIM_HandleTypeDef *const timer)
{
	ErrorEnum err = ERROR_OK;

   schedule_ = schedule;

   /* Timer for scheduler frames time measurement (in uS) */
   	  if (HAL_TIM_Base_Start(timer) != HAL_OK) {
   		  err = ERROR_HAL;
   	  }

   	if (err == ERROR_OK) {
		/* Assign scheduler timer to stop watch - no return */
		StopWatchInit(timer);

		SwTimerInitStats(&combinedStats_, UINT32_MAX);

	   for (uint32_t i = 0U; i < N_PROCESS; ++i)
	   {
		   SwTimerInitStats(&schedule_[i].stats, UINT32_MAX);
	   }

	   WatchdogFeed();
   	}

   	return err;
}

/**
 * @brief Scheduler callback.
 * 
 * The SchedulerCallback function is called by the system
 * every time the RTOS timer expires at a period of 1ms.
 */
void SchedulerCallback(void)
{
	  for (uint32 i = 0U; i < N_PROCESS; ++i) {
		  ++timers[i];
	  }
     /* Set elapsed flag to TRUE on callback */
	  hasElapsed = TRUE;
}

/**
 * @brief Scheduler runner infinite loop.
 */
void SchedulerRun(void)
{
   while (TRUE) {
      SchedulerTimeBase();
   }
}

/**
 * @brief Time base of the Scheduler.
 * 
 * If the system 1ms timer has elapsed, execute the tasks and reset the timer.
 */
static void SchedulerTimeBase(void)
{
   if (hasElapsed) {
      /* Execute Scheduler tasks */
      SchedulerExecuteTasks();
      /* Reset elapsed flag */
      hasElapsed = FALSE;
   }
}

/**
 * @brief Scheduler task execute function.
 * 
 * The function loops through the array of tasks and executes each task if its timer has expired.
 */
static void SchedulerExecuteTasks(void)
{
   static TimerStopWatchLapType frameStopWatch = {0U};
   static TimerStopWatchLapType taskStopWatch = {0U};

   TimerStopWatchStartFrame(&frameStopWatch);

   for (uint32_t i = 0U; i < N_PROCESS; ++i) {
         if (timers[i] >= schedule_[i].period) {
            //TimerStopWatchStartLap(&taskStopWatch);
            schedule_[i].handler();
            //SwTimerUpdateStats(&schedule_[i].stats, TimerStopWatchCaptureDuration(&taskStopWatch));
            timers[i] = 0U;
         }
   }

   const uint32_t frameDuration = TimerStopWatchCaptureDuration(&frameStopWatch);
   SwTimerUpdateStats(&combinedStats_, frameDuration);

   //SchedulerUpdateCpuLoadStats(frameDuration);
   WatchdogFeed();
}

#if 0
static void SchedulerUpdateCpuLoadStats(const uint32_t lastDuration)
{
#define MEASUREMENT_30S (30U)  /**< value of 30 second measurement*/
#define FRAMES_PER_MEASUREMENT (MEASUREMENT_30S * SYS_TICK_FREQ) /**< value of frame per measurement  */
#define CRITICAL_LOAD (50U)    /**< value of critical load */
#define FULL_LOAD (100U)       /**< value of full load */
#define CRITICAL_TIME ((CRITICAL_LOAD * MEASUREMENT_30S * HW_TIMER_FREQUENCY) / FULL_LOAD) /**< value of critical time */
#define SCALE_US_TO_MS (1000U) /**< value of microsecond in millisecond */

   static uint32_t totalDuration = 0U;
   static uint32_t framesCount = 0U;

   totalDuration += lastDuration;
   framesCount++;

   // Average processor load in the last 30 seconds
   if (framesCount >= FRAMES_PER_MEASUREMENT)
   {
      SchedulerUpdateStats(&accumulatedStats_, totalDuration);

      if (totalDuration > CRITICAL_TIME)
      {
         totalDuration /= SCALE_US_TO_MS;
         LoggerLogFault((uint8_t)FAULT_SCHEDULER_OVERLOAD, &totalDuration);
      }

      //Note for debugging: Utilization can be calculated by taking totalDuration and dividing by 30,000,000usecs
      totalDuration = 0U;
      framesCount = 0U;
   }
}
#endif

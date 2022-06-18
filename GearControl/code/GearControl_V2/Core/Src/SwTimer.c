// <BEGIN COPYRIGHT HEADER>
// * ===========================    Boeing AvionX    =========================== *
// * UNCLASSIFIED                                                                *
// * FOR OFFICIAL USE ONLY                                                       *
// * =========================================================================== *
// * BOEING PROPRIETARY.                                                         *
// * Copyright (c) 2021-2022 The Boeing Company.                                 *
// * All Rights Reserved.                                                        *
// * =========================================================================== *
// * EXPORT CONTROLLED - This technology or software is subject to the U.S.      *
// * Export Administration Regulations (EAR), (15 C.F.R. Parts 730-774). No      *
// * authorization from the U.S. Department of Commerce is required for export,  *
// * re-export, in-country transfer, or access EXCEPT to country group E:1 or    *
// * E:2 countries/persons per Supp.1 to Part 740 of the EAR. ECCN: 7D994        *
// * =========================================================================== *
// <END COPYRIGHT HEADER>

/**
 * @file SwTimer.c
 *
 * @brief Implementation of software timer
 */

#include "SwTimer.h"
#include "Types.h"
#include "main.h"

#define SW_TIMERS_MAX (10U) /**< Maximum number of SW Timers that can be registered*/
#define DEFAULT_COUNT (0U) /**< Default value of sw timer's counter  */

static SwTimerType* timers_[SW_TIMERS_MAX] = {0}; /**< Array of pointers to registered SW timers*/
static uint32 timersCount_ = 0U; /**< The number of registered SW timers */
static uint32 uptime_ = 0U; /**< System uptime in ticks */

/**
 * @brief Deactivate the software timer
 *
 * @param timer Software timer structure to deactivate
 *
 * @private
 */
static void SwTimerDeactivate(SwTimerType* timer);

/**
 * @brief Check if the given timer is expired
 *
 * @param timer Software timer structure to check for expiration
 *
 * @return True if the timer reaches the declared period
 *
 * @private
 */
static bool_t SwTimerIsExpired(const SwTimerType* timer);

/**
 * @brief Check if the given timer is active
 *
 * @param timer Software timer structure to check
 *
 * @return True if the software timer exists and is active
 *
 * @private
 */
static bool_t SwTimerIsActive(const SwTimerType* timer);

/**
 * @brief Deactivates software timer when the given timer is active and expired
 *
 * @param timer Software timer structure to process
 *
 * @private
 */
static void SwTimerProcessSwTimer(SwTimerType* timer);


void SwTimerExecute(void)
{
   ++uptime_;

   for (uint32 i = 0; i < timersCount_; i++)
   {
      SwTimerProcessSwTimer(timers_[i]);
   }
}

ErrorEnum SwTimerInit(SwTimerType* timer, uint32 period, bool_t endlessCount)
{
   //ASSERT_VOID(timer != NULL);
	ErrorEnum error = ERROR_OK;

	if (timer != NULL) {
	   timer->period = period / SYSTICK_RESOLUTION_IN_MS;
	   timer->endlessCount = endlessCount;
	   timer->active = false;
	   timer->count = DEFAULT_COUNT;
	   timer->elapsed = false;
	} else {
		error = ERROR_NULL;
	}

	return error;
}

uint32 SwTimerGetPeriod(const SwTimerType* timer)
{
   uint32 period = 0U;

   period = timer->period * SYSTICK_RESOLUTION_IN_MS;

   return period;
}

ErrorEnum SwTimerRegister(SwTimerType* timer)
{
	ErrorEnum err = ERROR_OK;

	if (timer != NULL) {
		if (timersCount_ < SW_TIMERS_MAX) {
		   timers_[timersCount_] = timer;
		   timersCount_++;
		} else {
			err = ERROR_SW_TIMER_MAX_EXCEEDED;
		}
	} else {
		err = ERROR_NULL;
	}

	return err;
}

void SwTimerStart(SwTimerType* timer)
{
   //ASSERT_VOID(timer != NULL);

   if (!timer->active)
   {
      timer->count = DEFAULT_COUNT;
      timer->elapsed = false;
      timer->active = true;
   }
}

bool_t SwTimerHasTimerElapsed(const SwTimerType* timer)
{
   //ASSERT_VOID(timer != NULL);

   return timer->elapsed;
}

uint32 SwTimerGetUptime(void)
{
   return uptime_;
}

static void SwTimerDeactivate(SwTimerType* timer)
{
   timer->count = 0U;
   timer->elapsed = true;

   if (!timer->endlessCount)
   {
      timer->active = false;
   }
}

static bool_t SwTimerIsExpired(const SwTimerType* timer)
{
   //A timer can have a period of 0, so this needs to be >=
   return timer->count >= timer->period;
}

static bool_t SwTimerIsActive(const SwTimerType* timer)
{
   bool_t isActive = false;

   if (timer != NULL)
   {
      if (timer->active)
      {
         isActive = true;
      }
   }

   return isActive;
}

static void SwTimerProcessSwTimer(SwTimerType* timer)
{
   if (SwTimerIsActive(timer))
   {
      //Clear the elapsed flag
      timer->elapsed = false;
      timer->count++;

      if (SwTimerIsExpired(timer))
      {
         SwTimerDeactivate(timer);
      }
   }
}

/**
 * @file SwTimer.c
 *
 * @brief Implementation of software timer
 */

#include "SwTimer.h"
#include "Utils.h"
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
 * @return TRUE if the timer reaches the declared period
 *
 * @private
 */
static bool_t SwTimerIsExpired(const SwTimerType* timer);

/**
 * @brief Deactivates software timer when the given timer is active and expired
 *
 * @param timer Software timer structure to process
 *
 * @private
 */
static void SwTimerProcessSwTimer(SwTimerType* timer);
static void SwTimerInit(SwTimerType* timer);

/**
 * @brief Process all Software Timers.
 */
void SwTimerExecute(void)
{
   for (uint32 i = 0; i < timersCount_; i++) {
      SwTimerProcessSwTimer(timers_[i]);
   }
}

void SwTimerTick1ms(void)
{
	++uptime_;
}

/**
 * The function SwTimerInit() initializes the timer by setting the active flag to FALSE, the count to
 * DEFAULT_COUNT, and the elapsed flag to FALSE.
 * 
 * @param timer A pointer to the timer to be initialized.
 */
static void SwTimerInit(SwTimerType* timer)
{
   timer->active = FALSE;
   timer->count = DEFAULT_COUNT;
   timer->elapsed = FALSE;
}

ErrorEnum SwTimerRegister(SwTimerType* timer)
{
	ErrorEnum err = ERROR_OK;

	if (NULL_CHECK1(timer)) {
		if (timersCount_ < SW_TIMERS_MAX) {
		   SwTimerInit(timer);
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

void SwTimerStart(SwTimerType* timer, const uint32_t period)
{
   if ((NULL_CHECK1(timer)) && (!timer->active)) {
	  timer->period = period / SYSTICK_RESOLUTION_IN_MS;
      timer->count = DEFAULT_COUNT;
      timer->elapsed = FALSE;
      timer->active = TRUE;
   }
}

bool_t SwTimerHasElapsed(const SwTimerType* timer)
{
   bool_t elapsed = FALSE;

   if (NULL_CHECK1(timer)) {
	   elapsed = timer->elapsed;
   }

   return elapsed;
}

uint32 SwTimerGetUptime(void)
{
   return uptime_;
}

/**
 * @param stats A pointer to the statistics structure.
 */
void SwTimerInitStats(__IO SwTimerStats *const stats, const uint32_t maxLimit)
{
   stats->lastT = 0U;
   stats->maxT = 0U;
   stats->minT = maxLimit;
   stats->maxLimit = maxLimit;
}

/**
 * @brief Update Sw Timer stats.
 *
 * It updates the statistics for a given task.
 *
 * @param stats    A pointer to the statistics structure for the task.
 * @param duration The time it took to execute the task.
 */
void SwTimerUpdateStats(__IO SwTimerStats* stats, uint32_t duration)
{
   duration = CLAMP_MAX(duration, stats->maxLimit);

   if (duration > stats->maxT) {
      stats->maxT = duration;
   }

   if (duration < stats->minT) {
      stats->minT = duration;
   }

   stats->lastT = duration;
}

static void SwTimerDeactivate(SwTimerType* timer)
{
   timer->count = 0U;
   timer->elapsed = TRUE;
   timer->active = FALSE;
}

static bool_t SwTimerIsExpired(const SwTimerType* timer)
{
   // A timer can have a period of 0, so this needs to be >=
   return timer->count >= timer->period;
}

bool_t SwTimerIsActive(const SwTimerType* timer)
{
   bool_t isActive = FALSE;

   if (NULL_CHECK1(timer)) {
      if (timer->active) {
         isActive = TRUE;
      }
   }

   return isActive;
}

static void SwTimerProcessSwTimer(SwTimerType* timer)
{
   if (SwTimerIsActive(timer)) {
      /* Clear the elapsed flag */
      timer->elapsed = FALSE;
      timer->count++;

      if (SwTimerIsExpired(timer)) {
         SwTimerDeactivate(timer);
      }
   }
}

void SwTimerDelay_Tick(__IO uint32_t* timer)
{
	if (NULL_CHECK1(timer)) {
		++(*timer);
	}
}

bool_t SwTimerDelay_Elapsed(__IO uint32_t* timer, const uint32_t timeout)
{
	bool_t elapsed = FALSE;

	if (NULL_CHECK1(timer)) {
		elapsed = (*timer >= timeout);
	}

	return elapsed;
}


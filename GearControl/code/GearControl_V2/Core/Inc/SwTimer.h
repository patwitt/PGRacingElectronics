/**
 * @file SwTimer.h
 *
 * @brief Public header for software timer
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_SW_TIMER_H
#define INC_SW_TIMER_H

#include "Types.h"

/**
  \brief  Structure used to track a Software Timer.
 */
typedef struct
{
   uint32 count;  /**< The current counter value */
   uint32 period; /**< The timer period in ticks */
   bool_t active;   /**< True if the timer is active (counting) */
   bool_t elapsed;  /**< True if the timer is elapsed */
   bool_t endlessCount; /**< If true, this timer will repeat counting after expiration */
} SwTimerType;

#define SYSTICK_RESOLUTION_IN_MS 10U /**< Systick's resolution value in milliseconds */
#define ONE_SEC_IN_MSEC 1000U /**< Value of second in milliseconds */

void SwTimerExecute(void);

/**
 * @brief Software Timer Initialization
 *
 * Initializes a single software timer with the given attributes.  If the timer is currently running, the timer will be stopped.
 *
 * @param timer Software timer structure to initialize
 * @param period Period of the timer in ms
 * @param endlessCount True if the timer should restart after expiring
 *
 * @note
 * The resolution of the timer is 10ms.  Everything will be rounded to the closest 10ms
 *
 */
ErrorEnum SwTimerInit(SwTimerType* timer, uint32 period, bool_t endlessCount);

/**
 * @brief Retrieves the period for a given timer
 *
 * @param timer Software timer structure to retrieve the period of
 *
 * @return the timer period in ms
 *
 */
uint32 SwTimerGetPeriod(const SwTimerType* timer);

/**
 * @brief Register a given software timer
 *
 * This method should only be called once per software timer during system initialization. Registering a timer multiple times will result in incorrect timing.
 *
 * @param timer Software timer structure to be registered
 *
 */
ErrorEnum SwTimerRegister(SwTimerType* timer);

/**
 * @brief Set default values and start the given timer
 *
 * @param timer Software timer structure to be started
 *
 */

void SwTimerStart(SwTimerType* timer);

/**
 * @brief Check if declared timer time elapsed
 *
 * @param timer Software timer structure to be checked if the time elapses, asserted to be != NULL
 *
 * @return True if the time has elapsed
 *
 */
bool_t SwTimerHasTimerElapsed(const SwTimerType* timer);

/**
 * @brief Returns the application uptime.
 *
 * This value will be in 10ms resolution. An uptime of 1 means 10ms of up-time.
 *
 * @return The number of 10ms ticks since the start of this application.
 *
 * @note This exists solely for logging. It should not be used for any timing functionality or calculations
 *
 */
uint32 SwTimerGetUptime(void);

#endif /* INC_SW_TIMER_H */

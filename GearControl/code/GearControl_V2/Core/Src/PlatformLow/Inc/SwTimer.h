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
  @brief  Structure used to track a Software Timer.
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

void SwTimerStart(SwTimerType* timer, const uint32_t period);

/**
 * @brief Check if the given timer is active
 *
 * @param timer Software timer structure to check
 *
 * @return True if the software timer exists and is active
 *
 *
 */
bool_t SwTimerIsActive(const SwTimerType* timer);

/**
 * @brief Check if declared timer time elapsed
 *
 * @param timer Software timer structure to be checked if the time elapses, asserted to be != NULL
 *
 * @return True if the time has elapsed
 *
 */
bool_t SwTimerHasElapsed(const SwTimerType* timer);

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

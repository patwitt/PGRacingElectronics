/*
 * MicroSwitch.h
 *
 *  Created on: 24.04.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_PLATFORMLOW_INC_SCHEDULER_H_
#define SRC_PLATFORMLOW_INC_SCHEDULER_H_

#include "Types.h"
#include "stm32f4xx_hal.h"

#define N_PROCESS (3U)

typedef void (*handleType)(void);

/**
* @brief Describes a statistics for the Scheduler.
*
* Structure used to collect statistics of each task
*
*/
typedef struct
{
	 uint32 lastDuration; /**< Last duration of task  */
	 uint32 minDuration;  /**< Minimal duration of task */
	 uint32 maxDuration;  /**< Maximum duration of task */
} SchedulerStatsType;

/**
* @brief Describes a task for the Scheduler.
*
* An array of these tasks will be registered with the Scheduler.
*
*/
typedef struct
{
  handleType handler; /**< Pointer to handler/execute function of task */
  uint32 period;
  SchedulerStatsType stats;  /**< Stores the task's statistics */
} SchedulerType;

ErrorEnum SchedulerInit(SchedulerType* const schedule, TIM_HandleTypeDef *const timer);
void SchedulerRun(void);
void SchedulerCallback(void);
void SysTickResetElapsed(void);
boolean SysTickHasElapsed(void);

#endif /* SRC_PLATFORMLOW_INC_SCHEDULER_H_ */

/*
 * InjectorsCut.h
 *
 *  Created on: 12.07.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_INJECTORS_CUT_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_INJECTORS_CUT_H_

#include "Types.h"
#include "DefineConfig.h"

#define INJECTORS_CUT_WATCHDOG_TIMEOUT_MS (105U)

#if CONFIG_GEAR_INJECTORS_CUT_ENABLE
#define INJECTORS_CUT_TRIGGER_DELAY_MS (10U)
#else
#define INJECTORS_CUT_TRIGGER_DELAY_MS (0U)
#endif

ErrorEnum InjectorsCut_Init(void);
void InjectorsCut_Trigger(void);
void InjectorsCut_Finish(void);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_INJECTORS_CUT_H_ */

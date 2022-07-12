/*
 * InjectorsCut.h
 *
 *  Created on: 12.07.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_INJECTORS_CUT_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_INJECTORS_CUT_H_

#include "Types.h"

ErrorEnum InjectorsCut_Init(void);
void InjectorsCut_Trigger(void);
void InjectorsCut_Finish(void);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_INJECTORS_CUT_H_ */

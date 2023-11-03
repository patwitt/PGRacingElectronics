/*
 * macros.h
 *
 *  Created on: 10 lis 2022
 *      Author: Patryk
 */

#ifndef MACROS_H_
#define MACROS_H_

#define MEASURE_FUNC_TIME(func) { int startTime = HAL_GetTick(); func; statsSave1(#func, startTime) }

#endif /* MACROS_H_ */

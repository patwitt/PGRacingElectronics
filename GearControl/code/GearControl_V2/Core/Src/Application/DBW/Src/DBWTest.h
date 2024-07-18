/*
 * DBWTest.h
 *
 *  Created on: 16 lip 2024
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_SRC_DBWTEST_H_
#define SRC_APPLICATION_DBW_SRC_DBWTEST_H_

#include "DBWTypes.h"

typedef enum {
	DBW_TEST_IDLE,
	DBW_TEST_START,
	DBW_TEST_RUNNING,
	DBW_TEST_FINISHED
} DBWTestState;

ErrorEnum DBWTest_Init(void);
void DBWTest_Process(void);
DBWTestState DBWTest_GetState(void);
float DBWTest_GetTpsTarget(void);

#endif /* SRC_APPLICATION_DBW_SRC_DBWTEST_H_ */

/*
 * DBWTest.c
 *
 *  Created on: 16 lip 2024
 *      Author: Patryk Wittbrodt
 */


#include "DBWTest.h"
#include "CAN.h"

#define RPM_TEST_START_TIMEOUT_MS (5000U)
#define RPM_TEST_STEP_TIMEOUT_MS (3000U)
#define RPM_TEST_START_RPM  (3000U)      // When below set RPM, start the test
#define RPM_TEST_CUTOFF_RPM (12000U)     // When RPM reached, end the test
#define RPM_TEST_INITIAL_TARGET (300.0f) // 30% Initial throttle target
#define RPM_TEST_THROTTLE_STEP (100.0f)  // 10% step increase
#define RPM_TEST_TARGET_MAX (1000.0f)    // Finish at 100% throttle opening

typedef struct {
	SwTimerType startTimer;
	SwTimerType stepTimer;
	uint32_t rpm;
	bool_t rpmDataNew;
	DBWTestState state;
	float tpsTarget;
	float nextTpsTarget;
} DBWTest;

static DBWTest test_ = {
		.state = DBW_TEST_IDLE,
		.tpsTarget = APPS_POS_MIN_F,
		.nextTpsTarget = RPM_TEST_INITIAL_TARGET
};

ErrorEnum DBWTest_Init(void)
{
	return SwTimerRegister(&test_.stepTimer) &&
		   SwTimerRegister(&test_.startTimer);
}

static inline void UpdateRpmData(void)
{
	uint8_t* emuBlackMsgBuff = CAN_GetRxNewData(CAN_RX_MSG_STDID_EMU_BLACK);

	if (emuBlackMsgBuff != NULL) {
		const uint32_t engineRPM = (uint32_t)emuBlackMsgBuff[CAN_DATA_BYTE_0];

		if ((engineRPM >= ENGINE_RPM_MIN) && (engineRPM <= ENGINE_RPM_MAX)) {
			test_.rpm = engineRPM;
			test_.rpmDataNew = TRUE;
		}
	}
}

static inline void UpdateTriggerData(void)
{
	uint8_t* triggerData = CAN_GetRxNewData(CAN_RX_MSG_STDID_GEAR_MODE);

	if (triggerData != NULL) {
		if (test_.state == DBW_TEST_IDLE) {
			SwTimerRestart(&test_.startTimer, RPM_TEST_START_TIMEOUT_MS);
			test_.state = DBW_TEST_START;
		} else {
			test_.state = DBW_TEST_FINISHED;
		}
	}
}

void DBWTest_Process(void)
{
	switch (test_.state) {
		case DBW_TEST_IDLE:
			/* Do nothing */
			break;

		case DBW_TEST_START:
			if (SwTimerHasElapsed(&test_.startTimer)) {
				/* Too long wait to start step of the test */
				test_.state = DBW_TEST_FINISHED;
			} else {
				if ((test_.rpm <= RPM_TEST_START_RPM) && (test_.rpmDataNew)) {
					/* Start test only on new RPM data */
					test_.rpmDataNew = FALSE;
					SwTimerRestart(&test_.stepTimer, RPM_TEST_STEP_TIMEOUT_MS);
					test_.state = DBW_TEST_RUNNING;
					test_.tpsTarget = test_.nextTpsTarget;
				} else {
					/* TPS back to IDLE */
					test_.tpsTarget = APPS_POS_MIN_F;
				}
			}
			break;

		case DBW_TEST_RUNNING:
			if (SwTimerHasElapsed(&test_.stepTimer) ||
				test_.rpm >= RPM_TEST_CUTOFF_RPM) {

				/* Throttle back to IDLE to restart with higher % opening or finish */
				test_.tpsTarget = APPS_POS_MIN_F;

				if (test_.nextTpsTarget < RPM_TEST_TARGET_MAX) {
					SwTimerRestart(&test_.startTimer, RPM_TEST_START_TIMEOUT_MS);
					test_.nextTpsTarget += RPM_TEST_THROTTLE_STEP;
					test_.state = DBW_TEST_START;
				} else {
					/* Test done */
					test_.state = DBW_TEST_FINISHED;
				}
			}
			break;

		case DBW_TEST_FINISHED:
		default:
			/* Reset */
			test_.tpsTarget = APPS_POS_MIN_F;
			test_.nextTpsTarget = RPM_TEST_INITIAL_TARGET;
			test_.state = DBW_TEST_IDLE;
			break;
	}
}

DBWTestState DBWTest_GetState(void)
{
	return test_.state;
}

float DBWTest_GetTpsTarget(void)
{
	return test_.tpsTarget;
}

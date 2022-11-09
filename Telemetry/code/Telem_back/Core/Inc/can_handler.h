/*
 * can.h
 *
 *  Created on: Aug 15, 2022
 *      Author: kisielski, Janicki
 */

#ifndef __CAN_PGR_H__
#define __CAN_PGR_H__

#include "stm32l4xx_hal.h"
#include "can.h"
#include "abs.h"
#include "amorki.h"
#include "break_pressure.h"

#define CAN_MSG_BYTES 4
#define CAN_TX_MAILBOX 0
#define DATA_LENGHT 2

#define BP_F_ID 0x600
#define BP_R_ID 0x601
#define DAMPER_LR_ID 0x602
#define DAMPER_RR_ID 0x603
#define ABS_LR_ID 0x604
#define ABS_RR_ID 0x605

extern HAL_StatusTypeDef can_status;


typedef struct {
	uint32_t msgId;
	uint8_t buffer[CAN_MSG_BYTES];
	HAL_StatusTypeDef halError;
} CAN_RxMsgType;


typedef struct {
	uint32_t msgId;
	float buffer[CAN_MSG_BYTES];
	uint32_t TxMailbox;
	HAL_CAN_StateTypeDef HalError;
} CAN_TxMsgType;


/* Init */
HAL_StatusTypeDef CAN_Init(CAN_HandleTypeDef* hcan);

/* TX functions */
HAL_StatusTypeDef CAN_TxSendData(const uint32_t txMsgId, float* dataBuffer,uint32_t TxMailbox);

/* RX functions */
HAL_StatusTypeDef CAN_GetRxData(const uint32_t rxMsgId, uint8_t dataBuffer[]);

/* RX Callback functions */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

/* CAN BREAK PRESSURE SENDING functions */
HAL_StatusTypeDef BP_F_over_can(BPSensor * sens);
HAL_StatusTypeDef BP_R_over_can(BPSensor * sens);

/* CAN VSS SENDING functions */
HAL_StatusTypeDef VSS_RR_over_can(ABSSensor * sens);
HAL_StatusTypeDef VSS_LR_over_can(ABSSensor * sens);

/* CAN DAMPER SENDING functions */
HAL_StatusTypeDef Damper_RR_over_can(DamperSensor * sens);
HAL_StatusTypeDef Damper_LR_over_can(DamperSensor * sens);

#endif /* INC_CAN_PGR_H_ */

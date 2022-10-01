/*
 * can.h
 *
 *  Created on: Aug 15, 2022
 *      Author: kisielski, Janicki
 */

#ifndef __CAN_PGR_H__
#define __CAN_PGR_H__

#include "stm32l4xx_hal.h"

#define CAN_MSG_BYTES 8
#define CAN_TX_MAILBOX 0

extern HAL_StatusTypeDef can_status;


typedef struct {
	uint32_t msgId;
	uint8_t buffer[CAN_MSG_BYTES];
	HAL_StatusTypeDef halError;
} CAN_RxMsgType;


typedef struct {
	uint32_t msgId;
	uint8_t buffer[CAN_MSG_BYTES];
	uint32_t TxMailbox;
	HAL_CAN_StateTypeDef HalError;
} CAN_TxMsgType;


/* Init */
HAL_StatusTypeDef CAN_Init(CAN_HandleTypeDef* hcan);

/* TX functions */
HAL_StatusTypeDef CAN_TxSendData(const uint32_t txMsgId, uint8_t* dataBuffer,uint32_t TxMailbox);

/* RX functions */
HAL_StatusTypeDef CAN_GetRxData(const uint32_t rxMsgId, uint8_t dataBuffer[]);

/* RX Callback functions */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

/* CAN BREAK PRESSURE SENDING functions */
HAL_StatusTypeDef Break_pressure_over_can(float BP[2]);



#endif /* INC_CAN_PGR_H_ */

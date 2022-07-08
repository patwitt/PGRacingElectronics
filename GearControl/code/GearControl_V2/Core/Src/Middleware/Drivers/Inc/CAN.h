/*
 * CAN.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_MIDDLEWARE_DRIVERS_INC_CAN_H_
#define SRC_MIDDLEWARE_DRIVERS_INC_CAN_H_

#include "Types.h"
#include "stm32f4xx_hal.h"

typedef enum {
	CAN_BYTE_STATUS   = 0x00U,
	CAN_BYTE_DATA_0   = 0x01U,
	CAN_BYTE_DATA_1   = 0x02U,
	CAN_BYTE_DATA_2   = 0x03U,
	CAN_BYTE_DATA_3   = 0x04U,
	CAN_BYTE_DATA_4   = 0x05U,
	CAN_BYTE_DATA_5   = 0x06U,
	CAN_BYTE_RESERVED = 0x07U, // CRC / TIMESTAMP
	CAN_BYTES_COUNT   = 0x08U
} CANMessageBytes;

typedef enum {
	CAN_STATUS_UNKOWN       = 0x00U,
	CAN_STATUS_INIT         = 0x01U,
	CAN_STATUS_OK           = 0xAAU,
	CAN_STATUS_NOT_RECEIVED = 0x10U,
	CAN_STATUS_ERROR        = 0xFFU
} CANMessageStatus;

typedef enum {
	CAN_RX_MSG_CLUTCH = 0x00U,
	CAN_RX_MSG_EMU_BLACK = 0x01U,
	CAN_RX_MSG_TELEMETRY = 0x02U,
	CAN_RX_MSG_COUNT = 0x03U
} CANRxMessagesEnum;

typedef enum {
	CAN_TX_MSG_GEARINFO = 0x00U,
	CAN_TX_MSG_COUNT = 0x01U
} CANTxMessagesEnum;

ErrorEnum CAN_Init(CAN_HandleTypeDef* hcan);
void CAN_TxCallback(void);
void CAN_RxCallback(void);
void CAN_UpdateTxData(const CANTxMessagesEnum txMsgId, const uint32_t byte, const uint8_t data);
void CAN_UpdateTxStatus(const CANTxMessagesEnum txMsgId, const CANMessageStatus status);
#endif /* SRC_MIDDLEWARE_DRIVERS_INC_CAN_H_ */

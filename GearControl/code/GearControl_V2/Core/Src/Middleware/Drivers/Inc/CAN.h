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
	CAN_BYTE_STATUS        = 0x00U,
	CAN_DATA_BYTE_0        = 0x01U,
	CAN_DATA_BYTE_1        = 0x02U,
	CAN_DATA_BYTE_2        = 0x03U,
	CAN_DATA_BYTE_3        = 0x04U,
	CAN_DATA_BYTE_4        = 0x05U,
	CAN_DATA_BYTE_5        = 0x06U,
	CAN_DATA_BYTE_RESERVED = 0x07U, // CRC / TIMESTAMP
	CAN_DATA_BYTES_COUNT   = 0x08U
} CAN_MsgDataBytes;

typedef enum {
	CAN_STATUS_UNKOWN       = 0x00U, //!< Failure
	CAN_STATUS_INIT         = 0x01U, //!< OK
	CAN_STATUS_ACK          = 0x03U, //!< OK
	CAN_STATUS_OK           = 0xAAU, //!< OK
	CAN_STATUS_NOT_RECEIVED = 0x10U, //!< Not sure yet about this
	CAN_STATUS_ERROR        = 0xFFU  //!< Failure
} CAN_MsgStatus;

typedef enum {
	CAN_RX_MSG_CLUTCH    = 0x00U,
	CAN_RX_MSG_EMU_BLACK = 0x01U,
	CAN_RX_MSG_TELEMETRY = 0x02U,
	CAN_RX_MSG_COUNT     = 0x03U
} CAN_RxMsgEnum;

typedef enum {
	CAN_TX_MSG_GEARINFO = 0x00U,
	CAN_TX_MSG_COUNT   = 0x01U
} CAN_TxMsgEnum;

typedef enum {
	CAN_RX_MSG_STDID_CLUTCH    = 0x00U,
	CAN_RX_MSG_STDID_EMU_BLACK = 0x01U,
	CAN_RX_MSG_STDID_TELEMETRY = 0x02U,
	CAN_RX_MSG_STDID_COUNT     = 0x03U
} CAN_RxMsgsStdIdEnum;

typedef enum {
	CAN_TX_MSG_STDID_GEARINFO = 0x300U
} CAN_TxMsgStdIdEnum;

/* Rx Message Type */
typedef struct {
	const CAN_RxMsgsStdIdEnum stdId;
	uint8_t buffer[CAN_DATA_BYTES_COUNT];
	bool_t newData;
	HAL_StatusTypeDef halErr;
	CAN_MsgStatus* status;
} CAN_RxMsgType;

/* Tx Message Type */
typedef struct {
	uint8_t buffer[CAN_DATA_BYTES_COUNT];
	HAL_StatusTypeDef error;
	CAN_TxHeaderTypeDef txHeader;
	CAN_MsgStatus* status;
} CAN_TxMsgType;

/* Init */
ErrorEnum CAN_Init(CAN_HandleTypeDef* hcan);

/* TX functions */
void CAN_TxCallback(void);
void CAN_TxUpdateData(const CAN_TxMsgEnum txMsgId, const CAN_MsgDataBytes byte, const uint8_t data);
void CAN_TxUpdateStatus(const CAN_TxMsgEnum txMsgId, const CAN_MsgStatus status);

/* RX functions */
void CAN_RxCallback(void);
CAN_RxMsgType* CAN_GetRxMsg(CAN_RxMsgEnum const rxMsgId);
ErrorEnum CAN_ValidateRxMsg(CAN_RxMsgType *const rxMsg);
uint8_t* CAN_GetRxNewData(CAN_RxMsgEnum const rxMsgId);

#endif /* SRC_MIDDLEWARE_DRIVERS_INC_CAN_H_ */

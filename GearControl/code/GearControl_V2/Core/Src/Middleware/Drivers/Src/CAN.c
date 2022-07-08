/*
 * CAN.c
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "CAN.h"
#include "main.h"
#include <string.h>

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

/* RX Messages ID's */
typedef enum {
	CAN_RX_MSG_ID_CLUTCH = 0x120U,
	CAN_RX_MSG_ID_EMU_BLACK = 0x121U,
	CAN_RX_MSG_ID_TELEMETRY = 0x122U
} CANRxMessageIdEnum;

/* TX Messages ID's */
typedef enum {
	CAN_TX_MSG_ID_GEARINFO = 0x120U
} CANTxMessageIdEnum;

/* Rx Message Type */
typedef struct {
	const CANRxMessageIdEnum msgId;
	uint8_t buffer[CAN_BYTES_COUNT];
	bool_t newData;
	HAL_StatusTypeDef error;
} CANRxMessageType;

/* Tx Message Type */
typedef struct {
	uint8_t buffer[CAN_BYTES_COUNT];
	HAL_StatusTypeDef error;
	CAN_TxHeaderTypeDef txHeader;
} CANTxMessageType;

/* CAN handler */
static CAN_HandleTypeDef* hcan_ = NULL;

static CANRxMessageType canRxMsgs_[CAN_RX_MSG_COUNT] = {
		[CAN_RX_MSG_CLUTCH] = {
			.msgId = CAN_RX_MSG_ID_CLUTCH,
			.error = HAL_OK,
			.newData = FALSE,
			.buffer = {0U}
		},
		[CAN_RX_MSG_EMU_BLACK] = {
			.msgId = CAN_RX_MSG_ID_EMU_BLACK,
			.error = HAL_OK,
			.newData = FALSE,
			.buffer = {0U}
		},
		[CAN_RX_MSG_TELEMETRY] = {
			.msgId = CAN_RX_MSG_ID_TELEMETRY,
			.error = HAL_OK,
			.newData = FALSE,
			.buffer = {0U}
		}
};

static CANTxMessageType canTxMsgs_[CAN_TX_MSG_COUNT] = {
		[CAN_TX_MSG_GEARINFO] = {
			.txHeader = {
				.DLC = CAN_BYTES_COUNT,
				.IDE = CAN_ID_STD,
				.RTR = CAN_RTR_DATA,
				.StdId = CAN_TX_MSG_ID_GEARINFO
			},
			.error = HAL_OK,
			.buffer = {0U}
		}
};

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */
static HAL_StatusTypeDef CAN_HALInit(CAN_HandleTypeDef* hcan);
static inline void CAN_UpdateRxMsg(CAN_RxHeaderTypeDef *const rxHeader,
		                           CANRxMessageType *const rxMsg,
								   const uint8_t *const rxBuffer,
								   const HAL_StatusTypeDef *const status);
static inline void CAN_DecodeRxMsg(CAN_RxHeaderTypeDef *const rxHeader,
		                           const uint8_t *const rxBuffer,
								   const HAL_StatusTypeDef *const status);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static HAL_StatusTypeDef CAN_HALInit(CAN_HandleTypeDef* hcan)
{
	static CAN_FilterTypeDef filterconfig;
	HAL_StatusTypeDef status = HAL_OK;

	filterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filterconfig.FilterIdHigh = 0x4;
	filterconfig.FilterIdLow = 0x0000;
	filterconfig.FilterMaskIdHigh = 0x4;
	filterconfig.FilterMaskIdLow = 0x0000;
	filterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
	filterconfig.FilterBank = 0;
	filterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	filterconfig.FilterActivation = ENABLE;

	status |= HAL_CAN_ConfigFilter(hcan, &filterconfig);
	status |= HAL_CAN_Start(hcan);
	status |= HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	return status;
}

static inline void CAN_UpdateRxMsg(CAN_RxHeaderTypeDef *const rxHeader, CANRxMessageType *const rxMsg, const uint8_t *const rxBuffer, const HAL_StatusTypeDef *const status)
{
	rxMsg->error = *status;

	if (rxMsg->error == HAL_OK) {
		memcpy((uint8_t*)rxMsg->buffer, (uint8_t*)rxBuffer, rxHeader->DLC);
		rxMsg->newData = TRUE;
	}
}

static inline void CAN_DecodeRxMsg(CAN_RxHeaderTypeDef *const rxHeader, const uint8_t *const rxBuffer, const HAL_StatusTypeDef *const status)
{
	switch (rxHeader->StdId) {
		case CAN_RX_MSG_ID_CLUTCH:
			CAN_UpdateRxMsg(rxHeader, &canRxMsgs_[CAN_RX_MSG_CLUTCH], rxBuffer, status);
			break;

		case CAN_RX_MSG_ID_EMU_BLACK:
			CAN_UpdateRxMsg(rxHeader, &canRxMsgs_[CAN_RX_MSG_EMU_BLACK], rxBuffer, status);
			break;

		case CAN_RX_MSG_ID_TELEMETRY:
			CAN_UpdateRxMsg(rxHeader, &canRxMsgs_[CAN_RX_MSG_TELEMETRY], rxBuffer, status);
			break;

		default:
			break;
	}
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum CAN_Init(CAN_HandleTypeDef* hcan)
{
	ErrorEnum error = ERROR_OK;

	if (hcan != NULL) {
		hcan_ = hcan;

		if (CAN_HALInit(hcan_) != HAL_OK) {
			error = ERROR_HAL;
		} else {
			/* Set CAN Statuses to INIT */
			for (uint32_t rxMsg = 0U; rxMsg < CAN_RX_MSG_COUNT; ++rxMsg) {
				canRxMsgs_[rxMsg].buffer[CAN_BYTE_STATUS] = CAN_STATUS_INIT;
			}
			for (uint32_t txMsg = 0U; txMsg < CAN_TX_MSG_COUNT; ++txMsg) {
				canTxMsgs_[txMsg].buffer[CAN_BYTE_STATUS] = CAN_STATUS_INIT;
			}
		}
	} else {
		error = ERROR_NULL;
	}

	return error;
}

void CAN_UpdateTxData(const CANTxMessagesEnum txMsgId, const uint32_t byte, const uint8_t data)
{
	if (txMsgId < CAN_TX_MSG_COUNT) {
		if ((byte < canTxMsgs_[txMsgId].txHeader.DLC) && (byte >= CAN_BYTE_DATA_0) && (byte <= CAN_BYTE_DATA_5)) {
			canTxMsgs_[txMsgId].buffer[byte] = data;
		}
	}
}

void CAN_UpdateTxStatus(const CANTxMessagesEnum txMsgId, const CANMessageStatus status)
{
	if (txMsgId < CAN_TX_MSG_COUNT) {
		canTxMsgs_[txMsgId].buffer[CAN_BYTE_STATUS] = status;
	}
}

void CAN_RxCallback(void)
{
	/* Local data - buffer and Rx Header */
	static uint8 canRxBuffer[CAN_BYTES_COUNT];
	static CAN_RxHeaderTypeDef RxHeader;

	/* Process Rx Message data */
	const HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(hcan_, CAN_RX_FIFO0, &RxHeader, canRxBuffer);
	CAN_DecodeRxMsg(&RxHeader, canRxBuffer, &status);
}

void CAN_TxCallback(void)
{
	static uint32_t TxMailbox = 0U;

	for (uint32_t canMsgId = 0U; canMsgId < CAN_TX_MSG_COUNT; ++canMsgId) {
		CANTxMessageType *const txMsg = &canTxMsgs_[canMsgId];

		txMsg->error = HAL_CAN_AddTxMessage(hcan_, &txMsg->txHeader, txMsg->buffer, &TxMailbox);

		if (txMsg->error != HAL_OK) {
			CAN_UpdateTxStatus(canMsgId, CAN_STATUS_ERROR);
		}
	}
}

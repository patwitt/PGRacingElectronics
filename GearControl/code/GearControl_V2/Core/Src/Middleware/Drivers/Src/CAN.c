/*
 * CAN.c
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "CAN.h"
#include "main.h"
#include "Utils.h"
#include <string.h>

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
/* RX messages */
static CAN_RxMsgType canRxMsgsConfig[CAN_RX_MSG_COUNT] = {
		[CAN_RX_MSG_CLUTCH] = {
			.stdId = CAN_RX_MSG_STDID_CLUTCH,
			.halErr = HAL_OK,
			.newData = FALSE,
			.buffer = {0U}
		},
		[CAN_RX_MSG_EMU_BLACK] = {
			.stdId = CAN_RX_MSG_STDID_EMU_BLACK,
			.halErr = HAL_OK,
			.newData = FALSE,
			.buffer = {0U}
		},
		[CAN_RX_MSG_TELEMETRY] = {
			.stdId = CAN_RX_MSG_STDID_TELEMETRY,
			.halErr = HAL_OK,
			.newData = FALSE,
			.buffer = {0U}
		}
};

/* TX messages */
static CAN_TxMsgType canTxMsgsConfig[CAN_TX_MSG_COUNT] = {
		[CAN_TX_MSG_GEARINFO] = {
			.txHeader = {
				.DLC = CAN_DATA_BYTES_COUNT,
				.IDE = CAN_ID_STD,
				.RTR = CAN_RTR_DATA,
				.StdId = CAN_TX_MSG_STDID_GEARINFO
			},
			.error = HAL_OK,
			.buffer = {0U}
		}
};

/* CAN Handler */
typedef struct {
	CAN_RxMsgType *const rxMsg;
	CAN_TxMsgType *const txMsg;
	CAN_HandleTypeDef* hcan;
} CAN_Handler;

static CAN_Handler canHandler_ = {.rxMsg = canRxMsgsConfig, .txMsg = canTxMsgsConfig, .hcan = NULL};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static HAL_StatusTypeDef CAN_HALInit(void);
static inline void CAN_UpdateRxMsg(CAN_RxHeaderTypeDef *const rxHeader,
		                           CAN_RxMsgType *const rxMsg,
								   const uint8_t *const rxBuffer,
								   const HAL_StatusTypeDef *const status);
static inline void CAN_DecodeRxMsg(CAN_RxHeaderTypeDef *const rxHeader,
		                           const uint8_t *const rxBuffer,
								   const HAL_StatusTypeDef *const status);
static ErrorEnum CAN_ValidateRxMsgStatus(const CAN_MsgStatus msgStatus);
/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static HAL_StatusTypeDef CAN_HALInit(void)
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

	status |= HAL_CAN_ConfigFilter(canHandler_.hcan, &filterconfig);
	status |= HAL_CAN_Start(canHandler_.hcan);
	status |= HAL_CAN_ActivateNotification(canHandler_.hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	return status;
}

static inline void CAN_UpdateRxMsg(CAN_RxHeaderTypeDef *const rxHeader, CAN_RxMsgType *const rxMsg, const uint8_t *const rxBuffer, const HAL_StatusTypeDef *const status)
{
	rxMsg->halErr = *status;

	if (rxMsg->halErr == HAL_OK) {
		memcpy((uint8_t*)rxMsg->buffer, (uint8_t*)rxBuffer, rxHeader->DLC);
		rxMsg->newData = TRUE;
	}
}

static inline void CAN_DecodeRxMsg(CAN_RxHeaderTypeDef *const rxHeader, const uint8_t *const rxBuffer, const HAL_StatusTypeDef *const status)
{
	switch (rxHeader->StdId) {
		case CAN_RX_MSG_STDID_CLUTCH:
			CAN_UpdateRxMsg(rxHeader, &canHandler_.rxMsg[CAN_RX_MSG_CLUTCH], rxBuffer, status);
			break;

		case CAN_RX_MSG_STDID_EMU_BLACK:
			CAN_UpdateRxMsg(rxHeader, &canHandler_.rxMsg[CAN_RX_MSG_EMU_BLACK], rxBuffer, status);
			break;

		case CAN_RX_MSG_STDID_TELEMETRY:
			CAN_UpdateRxMsg(rxHeader, &canHandler_.rxMsg[CAN_RX_MSG_TELEMETRY], rxBuffer, status);
			break;

		default:
			break;
	}
}

static ErrorEnum CAN_ValidateRxMsgStatus(const CAN_MsgStatus msgStatus)
{
	ErrorEnum err = ERROR_CAN_INVALID_STATUS;

	switch (msgStatus) {
		case CAN_STATUS_OK:
		case CAN_STATUS_INIT:
		case CAN_STATUS_ACK:
			err = ERROR_OK;
			break;

		default:
			break;
	}

	return err;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum CAN_Init(CAN_HandleTypeDef* hcan)
{
	ErrorEnum error = ERROR_OK;

	NULL_ERR_CHECK1(error, hcan);

	if (error == ERROR_OK) {
		/* Assign CAN handler pointer */
		canHandler_.hcan = hcan;

		if (CAN_HALInit() == HAL_OK) {
			/* Set CAN Status to INIT for Tx/Rx messages */
			for (uint32_t rxMsgId = 0U; rxMsgId < CAN_RX_MSG_COUNT; ++rxMsgId) {
				CAN_RxMsgType* rxMsg = &canHandler_.rxMsg[rxMsgId];
				rxMsg->status = (CAN_MsgStatus*)&rxMsg->buffer[CAN_BYTE_STATUS];
				*rxMsg->status = CAN_STATUS_INIT;
			}
			for (uint32_t txMsgId = 0U; txMsgId < CAN_TX_MSG_COUNT; ++txMsgId) {
				CAN_TxMsgType* txMsg = &canHandler_.txMsg[txMsgId];
				txMsg->status = (CAN_MsgStatus*)&txMsg->buffer[CAN_BYTE_STATUS];
				*txMsg->status = CAN_STATUS_INIT;
			}
		} else {
			error = ERROR_HAL;
		}
	}

	return error;
}

ErrorEnum CAN_ValidateRxMsg(CAN_RxMsgType *const rxMsg)
{
	ErrorEnum err = ERROR_OK;

	NULL_ERR_CHECK1(err, rxMsg);

	if (err == ERROR_OK) {
		NULL_ERR_CHECK1(err, rxMsg->status);

		if (err == ERROR_OK) {
			if (rxMsg->halErr == HAL_OK) {
				err = CAN_ValidateRxMsgStatus(*rxMsg->status);
			} else {
				err = ERROR_HAL;
			}
		}
	}

	return err;
}

CAN_RxMsgType* CAN_GetRxMsg(CAN_RxMsgEnum const rxMsgId)
{
	CAN_RxMsgType* rxMsgPtr = NULL;

	if (rxMsgId < CAN_RX_MSG_COUNT) {
		rxMsgPtr = &canHandler_.rxMsg[rxMsgId];
	}

	return rxMsgPtr;
}

void CAN_TxUpdateData(const CAN_TxMsgEnum txMsgId, const CAN_MsgDataBytes byte, const uint8_t data)
{
	if (txMsgId < CAN_TX_MSG_COUNT) {
		if ((byte < canHandler_.txMsg[txMsgId].txHeader.DLC) &&
			(byte >= CAN_DATA_BYTE_DATA_0) &&
			(byte <= CAN_DATA_BYTE_DATA_5)) {
			canHandler_.txMsg[txMsgId].buffer[byte] = data;
		}
	}
}

void CAN_TxUpdateStatus(const CAN_TxMsgEnum txMsgId, const CAN_MsgStatus status)
{
	if (txMsgId < CAN_TX_MSG_COUNT) {
		*canHandler_.txMsg[txMsgId].status = status;
	}
}

void CAN_RxCallback(void)
{
	/* Local data - buffer and Rx Header */
	static uint8 canRxBuffer[CAN_DATA_BYTES_COUNT];
	static CAN_RxHeaderTypeDef RxHeader;

	/* Process Rx Message data */
	const HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(canHandler_.hcan, CAN_RX_FIFO0, &RxHeader, canRxBuffer);
	CAN_DecodeRxMsg(&RxHeader, canRxBuffer, &status);
}

void CAN_TxCallback(void)
{
	static uint32_t TxMailbox = 0U;

	for (uint32_t canMsgId = 0U; canMsgId < CAN_TX_MSG_COUNT; ++canMsgId) {
		CAN_TxMsgType *const txMsg = &canHandler_.txMsg[canMsgId];

		txMsg->error = HAL_CAN_AddTxMessage(canHandler_.hcan, &txMsg->txHeader, txMsg->buffer, &TxMailbox);

		if (txMsg->error != HAL_OK) {
			CAN_TxUpdateStatus(canMsgId, CAN_STATUS_ERROR);
		}
	}
}

/*
 * CAN.c
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "CAN.h"
#include "DefineConfig.h"
#if CONFIG_ENABLE_CAN
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

static CAN_Handler canHandler_ = {
	.rxMsg = canRxMsgsConfig,
	.txMsg = canTxMsgsConfig,
	.hcan = NULL
};

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
/*        Local functions       */
/* ---------------------------- */

/**
 * @brief Initialization of the hardware abstraction layer of CAN bus.
 * 
 * Configure the CAN filter and enable the CAN peripheral.
 * 
 * @return CAN configuration status.
 */
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

/**
 * @brief Update RX message data.
 * 
 * It copies the received data from the hardware buffer to the software buffer.
 * 
 * @param rxHeader Pointer to the CAN_RxHeaderTypeDef structure.
 * @param rxMsg    Pointer to the CAN_RxMsgType struct that holds the received message.
 * @param rxBuffer Pointer to the buffer that contains the received data.
 * @param status   Status of HAL CAN message.
 */
static inline void CAN_UpdateRxMsg(CAN_RxHeaderTypeDef *const rxHeader, CAN_RxMsgType *const rxMsg, const uint8_t *const rxBuffer, const HAL_StatusTypeDef *const status)
{
	rxMsg->halErr = *status;

	if (rxMsg->halErr == HAL_OK) {
		memcpy((uint8_t*)rxMsg->buffer, (uint8_t*)rxBuffer, rxHeader->DLC);
		rxMsg->newData = TRUE;
	}
}

/**
 * @brief Decode Rx message by Standard ID.
 * 
 * @param rxHeader Pointer to the CAN_RxHeaderTypeDef struct.
 * @param rxBuffer Pointer to the buffer that contains the received data.
 * @param status   Status of HAL CAN message.
 */
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

/**
 * @brief Validate Rx Msg Status.
 * 
 * It checks if the message status is valid.
 * 
 * @param msgStatus The status of the message.
 * 
 * @return an error code.
 */
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

/**
 * @brief Initialization of the CAN module.
 * 
 * The function initializes the CAN peripheral and sets the status of all CAN messages to
 * CAN_STATUS_INIT.
 * 
 * @param hcan pointer to the CAN handle.
 * 
 * @return an error code.
 */
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

/**
 * @brief CAN Rx message validation.
 * 
 * It checks if the pointer to the CAN message is valid, then checks if the pointer to the status is
 * valid, then checks if the HAL error is OK, then checks if the status is valid.
 * 
 * @param rxMsg pointer to the CAN_RxMsgType structure.
 * 
 * @return an error code.
 */
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

/**
 * @brief Get Rx message new data.
 * 
 * If the message is valid and has new data, return the data buffer and clear the new data flag.
 * 
 * @param rxMsgId The ID of the message you want to read.
 * 
 * @return A pointer to the buffer of the message.
 */
uint8_t* CAN_GetRxNewData(CAN_RxMsgEnum const rxMsgId)
{
	CAN_RxMsgType *const rxMsg = CAN_GetRxMsg(rxMsgId);
	uint8_t* canDataBuff = NULL;

	if (CAN_ValidateRxMsg(rxMsg) == ERROR_OK) {
		if (rxMsg->newData) {
			canDataBuff = rxMsg->buffer;
			rxMsg->newData = FALSE;
		}
	}

	return canDataBuff;
}

/**
 * @brief Get Rx message struct pointer.
 * 
 * @param rxMsgId The ID of the message to get.
 * 
 * @return A pointer to the CAN_RxMsgType struct.
 */
CAN_RxMsgType* CAN_GetRxMsg(CAN_RxMsgEnum const rxMsgId)
{
	CAN_RxMsgType* rxMsgPtr = NULL;

	if (rxMsgId < CAN_RX_MSG_COUNT) {
		rxMsgPtr = &canHandler_.rxMsg[rxMsgId];
	}

	return rxMsgPtr;
}

/**
 * @brief Update Tx message data fields.
 * 
 * @param txMsgId The ID of the message to be sent.
 * @param byte    Byte to be updated.
 * @param data    The data to be sent.
 */
void CAN_TxUpdateData(const CAN_TxMsgEnum txMsgId, const CAN_MsgDataBytes byte, const uint8_t data)
{
		if ((byte < canHandler_.txMsg[txMsgId].txHeader.DLC) &&
			(byte >= CAN_DATA_BYTE_0) &&
			(byte <= CAN_DATA_BYTE_5)) {
			canHandler_.txMsg[txMsgId].buffer[byte] = data;
		}
}

/**
 * @brief Update the status of a CAN message.
 * 
 * @param txMsgId The ID of the message to update.
 * @param status  The status of the message.
 */
void CAN_TxUpdateStatus(const CAN_TxMsgEnum txMsgId, const CAN_MsgStatus status)
{
	if (txMsgId < CAN_TX_MSG_COUNT) {
		*canHandler_.txMsg[txMsgId].status = status;
	}
}

/**
 * @brief Rx message callback.
 * 
 * The CAN_RxCallback function is called by the CAN peripheral when a message is received. The function
 * then calls the HAL_CAN_GetRxMessage function to retrieve the message from the CAN peripheral. The
 * HAL_CAN_GetRxMessage function returns a status code that is passed to the CAN_DecodeRxMsg function.
 * The CAN_DecodeRxMsg function decodes the message and performs the appropriate action.
 */
void CAN_RxCallback(void)
{
	/* Local data - buffer and Rx Header */
	static uint8 canRxBuffer[CAN_DATA_BYTES_COUNT];
	static CAN_RxHeaderTypeDef RxHeader;

	/* Process Rx Message data */
	const HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(canHandler_.hcan, CAN_RX_FIFO0, &RxHeader, canRxBuffer);
	CAN_DecodeRxMsg(&RxHeader, canRxBuffer, &status);
}

/**
 * @brief Tx message callback.
 * 
 * If the CAN peripheral is ready to transmit, then transmit the message.
 * 
 * The first thing we do is check if the CAN peripheral is ready to transmit. If it is, then the 
 * message is transmitted.
 */
void CAN_TxCallback(void)
{
	static uint32_t TxMailbox = 3U;

	for (uint32_t canMsgId = 0U; canMsgId < CAN_TX_MSG_COUNT; ++canMsgId) {
		CAN_TxMsgType *const txMsg = &canHandler_.txMsg[canMsgId];

		txMsg->error = HAL_CAN_AddTxMessage(canHandler_.hcan, &txMsg->txHeader, txMsg->buffer, &TxMailbox);

		if (txMsg->error != HAL_OK) {
			CAN_TxUpdateStatus(canMsgId, CAN_STATUS_ERROR);
		}
	}
}
#else
ErrorEnum CAN_Init(CAN_HandleTypeDef* hcan) { (void)hcan; return ERROR_OK; }
ErrorEnum CAN_ValidateRxMsg(CAN_RxMsgType *const rxMsg) { (void) rxMsg; return ERROR_NULL; }
uint8_t* CAN_GetRxNewData(CAN_RxMsgEnum const rxMsgId) { (void)rxMsgId; return NULL; }
CAN_RxMsgType* CAN_GetRxMsg(CAN_RxMsgEnum const rxMsgId) { (void)rxMsgId; return NULL; }
void CAN_TxUpdateData(const CAN_TxMsgEnum txMsgId, const CAN_MsgDataBytes byte, const uint8_t data) { (void)txMsgId; (void)byte; (void)data; }
void CAN_TxUpdateStatus(const CAN_TxMsgEnum txMsgId, const CAN_MsgStatus status) { (void)txMsgId; (void)status; }
void CAN_RxCallback(void) {}
void CAN_TxCallback(void) {}
#endif // CONFIG_ENABLE_CAN

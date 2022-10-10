/*
 * can_pgr.c
 *
 *  Created on: Aug 15, 2022
 *      Author: kisielski, Janicki
 */

#include <can_handler.h>
#include "PTT_handler.h"

#define PTT_ADDRESS 0x208  //CAN STEERING WHEEL
#define TELEM_ADDRESS 0x601//CAN TELEMETRY BACK

static CAN_HandleTypeDef* hcan = NULL;
static CAN_FilterTypeDef filterconfig;
static uint32_t txMailbox;
static CAN_TxHeaderTypeDef TxHeader;
static CAN_RxHeaderTypeDef RxHeader;

HAL_StatusTypeDef can_status;

CAN_RxMsgType PTT;
CAN_TxMsgType TX_frame;

HAL_StatusTypeDef CAN_Init(CAN_HandleTypeDef* hcan_p)
{

	HAL_StatusTypeDef error = HAL_OK;

	if (hcan_p != NULL) {
		hcan = hcan_p;
		txMailbox = CAN_TX_MAILBOX;

		TxHeader.IDE = CAN_ID_STD;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.DLC = CAN_MSG_BYTES;

		filterconfig.FilterActivation = CAN_FILTER_ENABLE;
		filterconfig.FilterBank = 0;
		filterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
		filterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
		filterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
		filterconfig.FilterIdHigh = 0x0;
		filterconfig.FilterIdLow = 0x0000;
		filterconfig.FilterMaskIdHigh = 0x0;
		filterconfig.FilterMaskIdLow = 0x0000;

		error |= HAL_CAN_ConfigFilter(hcan, &filterconfig);
		error |= HAL_CAN_Start(hcan);
		error |= HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

		if (error != HAL_OK) {
			error = HAL_ERROR;
		}
	}
	else
	{
		error = HAL_ERROR;
	}

	return error;
}

/* TX functions */
HAL_StatusTypeDef CAN_TxSendData(const uint32_t txMsgId, uint8_t* dataBuffer, uint32_t TxMailbox)
{
	TxHeader.StdId = txMsgId;
	return HAL_CAN_AddTxMessage(hcan, &TxHeader, dataBuffer, &TxMailbox);
}

/* RX functions */
HAL_StatusTypeDef CAN_GetRxData(const uint32_t rxMsgId, uint8_t dataBuffer[])
{

	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, dataBuffer) == HAL_OK)
	{
		if(RxHeader.StdId == PTT_ADDRESS)	//PTT ID
	{
		PTT_control(dataBuffer);
	}

		return HAL_OK;
	}
	return HAL_ERROR;
}

HAL_StatusTypeDef Break_pressure_over_can(float BP[2])
{
    TxHeader.DLC = 4;  // data length
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.StdId = TELEM_ADDRESS;  // ID

    TX_frame.buffer[0] = BP[0];
    TX_frame.buffer[1] = BP[1];
    TX_frame.TxMailbox = CAN_TX_MAILBOX;
    can_status= HAL_CAN_AddTxMessage(hcan, &TxHeader, TX_frame.buffer, &TX_frame.TxMailbox);
    return can_status;
}

HAL_StatusTypeDef VSS_pressure_over_can(float VSS[2])
{
    TxHeader.DLC = 4;  // data length
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.StdId = TELEM_ADDRESS;  // ID

    TX_frame.buffer[0] = VSS[0];
    TX_frame.buffer[1] = VSS[1];
    TX_frame.TxMailbox = CAN_TX_MAILBOX;
    can_status= HAL_CAN_AddTxMessage(hcan, &TxHeader, TX_frame.buffer, &TX_frame.TxMailbox);
    return can_status;
}

HAL_StatusTypeDef VSS_pressure_over_can(float Damper[2])
{
    TxHeader.DLC = 4;  // data length
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.StdId = TELEM_ADDRESS;  // ID

    TX_frame.buffer[0] = Damper[0];
    TX_frame.buffer[1] = Damper[1];
    TX_frame.TxMailbox = CAN_TX_MAILBOX;
    can_status= HAL_CAN_AddTxMessage(hcan, &TxHeader, TX_frame.buffer, &TX_frame.TxMailbox);
    return can_status;
}

/* RX Callback functions */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)

{
	CAN_GetRxData(PTT.msgId, PTT.buffer);
}



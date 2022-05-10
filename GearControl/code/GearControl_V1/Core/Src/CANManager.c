/*
 * CANManager.c
 *
 *  Created on: 14.07.2021
 *      Author: User
 */

#include "main.h"
#include "CANManager.h"
#include "GearControl.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

/* CAN variables */
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
CAN_FilterTypeDef filterconfig;
uint32_t TxMailbox;
CAN_HandleTypeDef* _hcan;

/* Transmit and Receive Buffers */
uint8 receive_buff[CAN_FRAME_BYTES];
uint8 transmit_buff[CAN_FRAME_BYTES];

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

void CANManager_Init(CAN_HandleTypeDef* hcan)
{
	_hcan = hcan;
}

void CAN_ReceiveMsg(void)
{
	HAL_CAN_GetRxMessage(_hcan, CAN_RX_FIFO0, &RxHeader, receive_buff);
#if 0
  if (RxHeader.StdId == CAN_MSGID_DASHBOARD)
  {
	  if (receive_buff[0] == CAN_BUTTON_OFF)
	  {
		  GearControl_SetControl(GEAR_DISABLE);
	  }
	  else
	  {
		  GearControl_SetControl(GEAR_ENABLE);
	  }
  }
#endif
}

void CAN_TransmitMsg(uint8* data)
{
	HAL_CAN_AddTxMessage(_hcan, &TxHeader, data, &TxMailbox);
}

HAL_StatusTypeDef CAN_Init(void)
{
	HAL_StatusTypeDef status = 0;

	TxHeader.DLC = 8; /* Data frame length */
	TxHeader.IDE = CAN_ID_STD; /* Frame ID type (standard) */
	TxHeader.RTR = CAN_RTR_DATA; /* Frame type */
	TxHeader.StdId = 1; /* standard ID, od 0 do 0x7FF */

	filterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filterconfig.FilterIdHigh = 0x4;
	filterconfig.FilterIdLow = 0x0000;
	filterconfig.FilterMaskIdHigh = 0x4;
	filterconfig.FilterMaskIdLow = 0x0000;
	filterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
	filterconfig.FilterBank = 0;
	filterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	filterconfig.FilterActivation = ENABLE;

	status |= HAL_CAN_ConfigFilter(_hcan, &filterconfig);
	status |= HAL_CAN_Start(_hcan);
	status |= HAL_CAN_ActivateNotification(_hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	return status;
}

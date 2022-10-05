/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    can.c
 * @brief   This file provides code for the configuration
 *          of the CAN instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#define __USE_XOPEN
#define _GNU_SOURCE
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
#include "structs/ecumaster.h"
#include "structs/telemetry_data.h"
#include "structs/display_data.h"
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

uint32_t TxMailbox;

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 9;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_2TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
	CAN_FilterTypeDef canfilterconfig;
	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank = 8; // which filter bank to use from the assigned ones
	canfilterconfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	canfilterconfig.FilterIdHigh = 0;
	canfilterconfig.FilterIdLow = 0;
	canfilterconfig.FilterMaskIdHigh = 0;
	canfilterconfig.FilterMaskIdLow = 0;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
	canfilterconfig.SlaveStartFilterBank = 10;
	HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);
	//HAL_CAN_Start(&hcan1);

  /* USER CODE END CAN1_Init 2 */

}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 9;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_2TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = ENABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */
	CAN_FilterTypeDef canfilterconfig;
	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank = 18; // which filter bank to use from the assigned ones
	canfilterconfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	canfilterconfig.FilterIdHigh = 0;
	canfilterconfig.FilterIdLow = 0;
	canfilterconfig.FilterMaskIdHigh = 0;
	canfilterconfig.FilterMaskIdLow = 0;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
	canfilterconfig.SlaveStartFilterBank = 10;
	HAL_CAN_ConfigFilter(&hcan2, &canfilterconfig);

	//HAL_CAN_Start(&hcan2);

  /* USER CODE END CAN2_Init 2 */

}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED=0;

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* CAN2 clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8];

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
	if (hcan->Instance == INTERNAL_CAN)
	{
		//ComputeEcumasterFrame(RxHeader, RxData);
		ComputeInternalFrame(RxHeader, RxData);
	}
	else
	{
		//ComputeInternalFrame(RxHeader, RxData);
		ComputeEcumasterFrame(RxHeader, RxData);
	}
}

void CanCommunicationInit()
{
	HAL_CAN_Start(&hcan2);
	HAL_CAN_Start(&hcan1);

	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

	CAN_TxHeaderTypeDef TxHeader;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 8;
	TxHeader.StdId = GIT_BUILD_TAG;
	uint8_t TxData[8] =
	{ 1, 0, 0, 0, 0, 0, 0, 0 };

	HAL_CAN_AddTxMessage(&INTERNAL_CAN_HANDLE, &TxHeader, TxData, &TxMailbox);

	TxHeader.DLC = 4;
	TxHeader.StdId = GIT_BUILD_TAG;

	sscanf(_V_BUILD_TAG, "v%hu.%hu.%hu", (short unsigned*) &TxData[0],
			(short unsigned*) &TxData[1], (short unsigned*) &TxData[2]);
	TxData[3] = _V_COMMITS_AHEAD;

#ifdef _V_GIT_DIRTY_FLAG
	TxData[4] = 1;
#endif
	HAL_CAN_AddTxMessage(&INTERNAL_CAN_HANDLE, &TxHeader, TxData, &TxMailbox);

	char Temp[10];
	TxHeader.DLC = 8;
	TxHeader.StdId = GIT_COMMIT;

	memset(TxData, 0, 8);
	strcpy(Temp, _V_COMMIT);
	strncpy((char*) TxData, Temp, 8);
	HAL_CAN_AddTxMessage(&INTERNAL_CAN_HANDLE, &TxHeader, TxData, &TxMailbox);

	// parse build date and send through can
	struct tm result;
	strptime(_V_DATE, "%d.%m.%Y %R", &result);
	time_t tempTime = mktime(&result);

	uint8_t counter = 0;
	while (tempTime > 0 && counter < 8)
	{
		TxData[counter++] = tempTime % 0xFF;
		tempTime /= 0xFF;
	}

	TxHeader.StdId = BUILD_DATE;
	HAL_CAN_AddTxMessage(&INTERNAL_CAN_HANDLE, &TxHeader, TxData, &TxMailbox);
}

void ComputeEcumasterFrame(CAN_RxHeaderTypeDef RxHeader, uint8_t *RxData)
{
	switch (RxHeader.StdId)
	{
	case Frame1:
		ecumasterData.rpm = LittleToBigEndian(&RxData[0]);
		ecumasterData.tps = RxData[2];
		ecumasterData.iat = RxData[3];
		ecumasterData.map = LittleToBigEndian(&RxData[4]);
		ecumasterData.injPW = LittleToBigEndian(&RxData[6]);
		break;
	case Frame2:
		ecumasterData.ain1 = LittleToBigEndian(&RxData[0]) * 0.0048828125f;
		ecumasterData.ain2 = LittleToBigEndian(&RxData[2]) * 0.0048828125f;
		ecumasterData.ain3 = LittleToBigEndian(&RxData[4]) * 0.0048828125f;
		ecumasterData.ain4 = LittleToBigEndian(&RxData[6]) * 0.0048828125f;
		break;
	case Frame3:
		ecumasterData.speed = LittleToBigEndian(&RxData[0]);
		ecumasterData.oilTemp = RxData[3];
		ecumasterData.oilPress = RxData[4] * 0.0625f;
		ecumasterData.clt = LittleToBigEndian(&RxData[6]);
		break;
	case Frame4:
		ecumasterData.ignAngle = RxData[0];
		ecumasterData.ignDwell = RxData[1];
		ecumasterData.lambda = RxData[2];
		ecumasterData.lambdaCorrection = RxData[3];
		ecumasterData.egt1 = LittleToBigEndian(&RxData[4]);
		ecumasterData.egt2 = LittleToBigEndian(&RxData[6]);
		break;
	case Frame5:
		ecumasterData.gear = RxData[0];
		ecumasterData.ecuTemp = RxData[1];
		ecumasterData.batt = LittleToBigEndian(&RxData[2]) * 0.027f;
		ecumasterData.errflag = LittleToBigEndian(&RxData[5]);
		ecumasterData.flags1 = RxData[7];
		break;
	case Frame6:
		ecumasterData.DBWPosition = RxData[0];
		ecumasterData.DBWTrigger = RxData[1];
		ecumasterData.TCDRPMRaw = LittleToBigEndian(&RxData[2]);
		ecumasterData.TCDRPM = LittleToBigEndian(&RxData[4]);
		ecumasterData.TCTorqueReduction = RxData[6];
		ecumasterData.PitLimitTorqueReduction = RxData[7];
		break;
	case Frame7:
		break;
	case Frame8:
		break;
	case 768:
		telemetryData.gear = RxData[1];
		telemetryData.minGearChangeTime = RxData[2];
		telemetryData.gearChangeTime = RxData[3];
		telemetryData.maxGearChangeTime = RxData[4];
		if (telemetryData.gear == 0)
		{
			telemetryData.gear = 1;

		}
		else if (telemetryData.gear == 1)
		{
			telemetryData.gear = 0;
		}

		break;
	default:
		;
// forward frame to internal can
		CAN_TxHeaderTypeDef TxHeader;
		TxHeader.DLC = RxHeader.DLC;
		TxHeader.IDE = RxHeader.IDE;
		TxHeader.RTR = RxHeader.RTR;
		TxHeader.StdId = RxHeader.StdId;
		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, RxData, &TxMailbox);
	}
}

void ComputeInternalFrame(CAN_RxHeaderTypeDef RxHeader, uint8_t *RxData)
{
	if (RxHeader.StdId == 0x520)
	{
		telemetryData.isIntercomActive = RxData[0] & 0x8 ? 1 : 0;
	}
	else if (RxHeader.StdId == 0x500)
	{
		telemetryData.steeringWheelAttached = 1;
		telemetryData.steeringWheelAttachedTimestamp = HAL_GetTick();
	}
}

uint16_t LittleToBigEndian(uint8_t *data)
{
	uint16_t returnData = data[0] + (data[1] << 8);
	return returnData;
}

/* USER CODE END 1 */

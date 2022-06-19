/*
 * CANManager.h
 *
 *  Created on: 14.07.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_MIDDLEWARE_DRIVERS_INC_CAN_H_
#define SRC_MIDDLEWARE_DRIVERS_INC_CAN_H_

#include "Types.h"
#include "stm32f4xx_hal.h"

#define CAN_BUTTON_OFF (0xFFU)
#define CAN_MSGID_DASHBOARD (0x02U)
#define CAN_FRAME_BYTES (8U)

void CAN_Init(CAN_HandleTypeDef*);
HAL_StatusTypeDef CAN_HALInit(void);
void CAN_TransmitMsg(uint8* data);
void CAN_ReceiveMsg(void);
#endif /* SRC_MIDDLEWARE_DRIVERS_INC_CAN_H_ */

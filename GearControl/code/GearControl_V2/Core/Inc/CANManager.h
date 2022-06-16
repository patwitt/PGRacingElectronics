/*
 * CANManager.h
 *
 *  Created on: 14.07.2021
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_CANMANAGER_H_
#define INC_CANMANAGER_H_

#define CAN_BUTTON_OFF (0xFFU)
#define CAN_MSGID_DASHBOARD (0x02U)
#define CAN_FRAME_BYTES (8U)

void CANManager_Init(CAN_HandleTypeDef*);
HAL_StatusTypeDef CAN_Init(void);
void CAN_TransmitMsg(uint8* data);
void CAN_ReceiveMsg(void);
#endif /* INC_CANMANAGER_H_ */

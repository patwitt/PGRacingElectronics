/*
 * ClutchControl.h
 *
 *  Created on: 11.07.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_CLUTCHCONTROL_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_CLUTCHCONTROL_H_

#include "Types.h"
#include "stm32f4xx_hal.h"

typedef enum {
	CLUTCH_CTRL_CAN_ERROR      = 0U, //!< CAN error - still operational, position set to default
	CLUTCH_CTRL_SERVO_DISABLED = 1U, //!< Servo error - not operational, servo disabled
	CLUTCH_CTRL_INIT_CAN_ACK   = 2U, //!< Wait for CAN message to acknowledge communication
	CLUTCH_CTRL_NORMAL_OP      = 3U, //!< Normal operation (controlled from CAN bus)
	CLUTCH_CTRL_GEARCTRL       = 4U, //!< Controlled from Gear Control module
	CLUTCH_CTRL_COUNT          = 5U
} ClutchControlStates;

ErrorEnum ClutchControl_Init(TIM_HandleTypeDef *const htim);
void ClutchControl_Process(void);
void ClutchControl_SetState(const ClutchControlStates clutchControl);

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_CLUTCHCONTROL_H_ */

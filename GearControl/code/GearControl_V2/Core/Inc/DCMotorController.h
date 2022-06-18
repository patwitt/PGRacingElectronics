/*
 * DCMotorController.h
 *
 *  Created on: 18.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_DCMOTORCONTROLLER_H_
#define INC_DCMOTORCONTROLLER_H_

#include "Types.h"

typedef enum {
	DC_MOTOR_ROTATE_MINUS = 0U,
	DC_MOTOR_ROTATE_PLUS  = 1U,
	DC_MOTOR_DISABLED     = 2U
} DCMotorDirectionEnum;

ErrorEnum DCMotorController_Init(void);
void DCMotorController_Disable(void);
void DCMotorController_Enable(void);
void DCMotorController_Update(float target, DCMotorDirectionEnum direction);

#endif /* INC_DCMOTORCONTROLLER_H_ */

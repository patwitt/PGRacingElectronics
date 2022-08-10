/*
 * ClutchControl.h
 *
 *  Created on: 11.07.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_CLUTCHCONTROL_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_CLUTCHCONTROL_H_

#include "Types.h"
#include "DefineConfig.h"
#include "stm32f4xx_hal.h"

#define CLUTCH_UPSHIFT_SLIP_TIMEOUT_MS   (80U)
#define CLUTCH_DOWNSHIFT_SLIP_TIMEOUT_MS (80U)
#define CLUTCH_TIMEOUT_MS                (600U)

#define CLUTCH_UPSHIFT_SLIP_DEG (124U)
#define CLUTCH_DOWNSHIFT_SLIP_DEG (124U)

#if CONFIG_ENABLE_CLUTCH && CONFIG_ENABLE_CLUTCH_SLIP
#define CLUTCH_UPSHIFT_DELAY_MS (20U)
#define CLUTCH_DOWNSHIFT_DELAY_MS (20U)
#else
#define CLUTCH_UPSHIFT_DELAY_MS (0U)
#define CLUTCH_DOWNSHIFT_DELAY_MS (0U)
#endif

typedef enum {
	CLUTCH_DIR_UPSHIFT,
	CLUTCH_DIR_DOWNSHIFT,
	CLUTCH_DIR_COUNT
} ClutchShiftDirection;

typedef struct {
	const uint32_t slipDeg;
	const uint32_t slipDelayMs;
	const ClutchShiftDirection direction;
} ClutchSlipConfig;

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
void ClutchControl_SetControl(const ClutchControlStates clutchControl);
bool_t ClutchControl_IsEngaged(void);
void ClutchControl_TriggerSlip(const uint32_t slipDegrees, const ClutchShiftDirection direction);
void ClutchControl_DisableSlip(void);
#endif /* SRC_APPLICATION_GEARCONTROL_SRC_CLUTCHCONTROL_H_ */

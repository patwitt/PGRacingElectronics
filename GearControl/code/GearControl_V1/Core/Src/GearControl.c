/*
 * GearControl.c
 *
 *  Created on: 15.03.2021
 *      Author: Patryk Wittbrodt
 */
#include "Encoder.h"
#include "GearControl.h"
#include "MicroSwitch.h"
#include "GearSensor.h"
#include "stm32f1xx_it.h"
#include "main.h"
#include <math.h>
#include "CANManager.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

typedef struct
{
	__IO ShiftStates state;
	__IO uint8 request;
	__IO uint8 validation;
}ShiftCtrl;

typedef struct
{
	GearStates state;
	uint8 degreesUp;
	uint8 degreesDown;
}GearData;

#define DEG_1_TO_N (32U)
#define DEG_UP (80U)
#define DEG_DOWN (DEG_UP + 5U)
#define DEG_N_DOWN (70U)
#define DEG_N_UP (86U)
#define DEG_1_2 (90U)

uint8 check_valid = 0;

static const GearData GearDataDecoder[] = {
		{GEAR_INIT, 0U,        0U},
		{GEAR_1,    DEG_1_2,   0U},
		{GEAR_N,    DEG_N_DOWN,DEG_N_UP},
		{GEAR_2,    DEG_UP,    DEG_1_2},
		{GEAR_3,    DEG_UP,    DEG_DOWN},
		{GEAR_4,    DEG_UP,    DEG_DOWN},
		{GEAR_5,    DEG_UP,    DEG_DOWN},
		{GEAR_6,    0U,        DEG_DOWN}
};

typedef struct
{
	__IO uint32* PWM;
	__IO uint32 PwmRequest;
	__IO uint32 errors;
	__IO uint8 wdgCnt;
	__IO GearStates prevGear;
	__IO GearStates state;
	__IO GearStates nextState;
	__IO GearSensorStates GearSensState;
	__IO boolean wdgStatus;
	__IO boolean activation;
	__IO boolean cutStatus;
	const GearData* data;
}GearCtrl;

static __IO ShiftCtrl ShiftHandler = {.state = SHIFT_INIT, .request = 0, .validation = FALSE};
static __IO GearCtrl GearHandler = {.PWM = NULL,
		                            .PwmRequest = 0,
									.errors = 0,
									.wdgCnt = 0,
									.prevGear = GEAR_INVALID,
									.state = GEAR_INIT,
									.nextState = GEAR_INVALID,
									.GearSensState = GEAR_IDLE,
									.wdgStatus = FALSE,
									.data = NULL,
									.activation = GEAR_ENABLE,
									.cutStatus = FALSE};
static uint8 can_data[CAN_FRAME_BYTES] = {0};
static __IO uint32 ticks = 0;

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */

static GearStates GearState_Init(void);
static GearStates GearStateHandler(void);

static boolean GearControl_PrepareCANMessage(void);
static inline boolean GearState_ValidateRequest(uint8);

static void GearControl_WdgOff(void);
static void GearControl_RequestGearChange(__IO uint8*, GearStates*);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */

/* Reset WDG parameters */
static void GearControl_WdgOff(void)
{
	GearHandler.wdgCnt = 0U;
	GearHandler.wdgStatus = FALSE;
}

/* Gear state init handler */
static GearStates GearState_Init(void)
{
	static GearStates initGear = GEAR_INVALID;
	static InitStates initState = INIT_SERVO;
	static uint8 cnt = 0;
	GearStates nextState = GEAR_INIT;

	switch (initState)
	{
		case INIT_SERVO:
			GearControl_SetServoPos(SET_DEG, POS_DEG_DEFAULT, 0);
			initState = INIT_CAN_BUS;
			break;

		case INIT_CAN_BUS:
			++cnt;

			if (HAL_OK == CAN_Init())
			{
				#ifdef READ_ENCODER
					initState = INIT_ENCODER;
				#else
					initState = INIT_ADC;
				#endif
			}
			else
			{
				if (cnt > INIT_ATTEMPTS)
				{
					HAL_Delay(5000);
					cnt = 0;
					NVIC_SystemReset();
				}
			}
			break;

		case INIT_ENCODER:
			++cnt;
			if (cnt < INIT_ATTEMPTS)
			{
				if (TRUE == Encoder_Init())
				{
					cnt = 0;
					initState = INIT_ADC;
				}
			}
			else
			{
				/* Error */
				GearControl_SetError(INIT_ENCODER_ERROR, TRUE);
				cnt = 0;
			}
			break;


		case INIT_ADC:
			initGear = GearSensor_GetStateBySensorAdc(N_SAMPLES);

			if (GEAR_INVALID != initGear)
			{
				GearHandler.prevGear = initGear;
				can_data[0] = initGear;
				can_data[1] = COL_BLACK;
				cnt = 0;
				initState = INIT_DONE;
			}
			else
			{
				++cnt;
			}

			if (cnt > INIT_ATTEMPTS)
			{
				cnt = 0;
				can_data[0] = 0;
				can_data[1] = COL_ORANGE;
				CAN_TransmitMsg(can_data);
			}
			break;

		case INIT_DONE:
			CAN_TransmitMsg(can_data);
			nextState = initGear;
			initState = INIT_SERVO;
			initGear = GEAR_INVALID;
			cnt = 0;
			break;

		default:
			break;
	}

	return nextState;
}

static inline uint8 GearState_ValidateRequest(uint8 request)
{
	uint8 ret = REQUEST_EMPTY;

	switch (GearHandler.state)
	{
		case GEAR_1:
			if (REQUEST_SHIFT_UP == request)
			{
				ret = request;
			}
			else
			{
				ret = REQUEST_SHIFT_N;
			}
			break;

		case GEAR_N:
		case GEAR_2:
		case GEAR_3:
		case GEAR_4:
		case GEAR_5:
			if ((REQUEST_SHIFT_UP == request) ||
				(REQUEST_SHIFT_DOWN == request))
			{
				ret = request;
			}
			break;

		case GEAR_6:
			if (REQUEST_SHIFT_DOWN == request)
			{
				ret = request;
			}
			break;

		default:
			break;
	}

	return ret;
}

static void GearControl_RequestGearChange(__IO uint8* request, GearStates* savedState)
{
	/* Valid request, change gear */
	switch (*request)
	{
		case REQUEST_SHIFT_DOWN:
			GearControl_SetServoPos(SET_DEG, (POS_DEG_DEFAULT - GearHandler.data->degreesDown), 0);
			if (GEAR_2 == GearHandler.state)
			{
				/* On normal request from 2 skip N to 1 gear */
				*savedState = (GearStates)((uint8)GearHandler.state - 2U);
			}
			else
			{
				*savedState = (GearStates)((uint8)GearHandler.state - 1U);
			}
			break;

		case REQUEST_SHIFT_UP:
			GearControl_SetServoPos(SET_DEG, (POS_DEG_DEFAULT + GearHandler.data->degreesUp), 0);
			if (GEAR_1 == GearHandler.state)
			{
				/* On normal request from 1 skip N to 2 gear */
				*savedState = (GearStates)((uint8)GearHandler.state + 2U);
			}
			else
			{
				*savedState = (GearStates)((uint8)GearHandler.state + 1U);
			}
			break;

		case REQUEST_SHIFT_N:
			GearControl_SetServoPos(SET_DEG, (POS_DEG_DEFAULT + DEG_1_TO_N), 0);
			*savedState = (GearStates)((uint8)GearHandler.state + 1U);
			break;

		default:
			break;
	}
}

/* CAN FRAME: */
/* 1 byte - gear */
/* 2 byte - status colour */
static boolean GearControl_PrepareCANMessage(void)
{
	static uint8 prevGearMsg = 0;
	static uint8 prevColMsg = 0;
	static uint16 idle_ticks = 0;
	boolean send = FALSE;

	if (ShiftHandler.validation == VALIDATION_DONE)
	{
		switch (GearHandler.GearSensState)
		{
			case GEAR_OK:
				ShiftHandler.validation = VALIDATION_END;
				can_data[0] = GearHandler.state;
				can_data[1] = COL_GREEN;
				break;

			case GEAR_UNKNOWN:
				ShiftHandler.validation = VALIDATION_POLL;
				can_data[0] = GearHandler.prevGear;
				can_data[1] = COL_ORANGE;
				break;

			case GEAR_NOT_CHANGED:
				ShiftHandler.validation = VALIDATION_END;
				can_data[0] = GearHandler.state;
				can_data[1] = COL_RED;
				break;

			case GEAR_IDLE:
			default:
				break;
		}

		GearHandler.GearSensState = GEAR_IDLE;
	}

	/* Gear Change disabled, send grey status */
	if (FALSE == GearControl_IsActive())
	{
		can_data[1] = COL_GREY;
	}
	else
	{
		can_data[1] = (can_data[1] == COL_GREY) ? COL_BLACK : can_data[1];
	}

	if ((can_data[0] != prevGearMsg) || (can_data[1] != prevColMsg))
	{
		send = TRUE;
		idle_ticks = 0;
	}
	else
	{
		if ((++idle_ticks) > DELAY_720MS)
		{
			/* Set colour to black after 1 second of inactivity */
			if ((can_data[1] != COL_ORANGE) &&
				(can_data[1] != COL_GREY))
			{
				can_data[1] = COL_BLACK;
			}
			send = TRUE;
			idle_ticks = 0;
		}
	}

	prevGearMsg = can_data[0];
	prevColMsg = can_data[1];

	return send;
}

/* Gear state handler, called every 4 ms */
static GearStates GearStateHandler(void)
{
	static GearStates savedState = GEAR_INVALID;
	static uint8 fuelCutDelay = 0U;
	GearStates nextState = GearHandler.state;

	switch (ShiftHandler.state)
	{
		case SHIFT_INIT:
			ShiftHandler.state = SHIFT_IDLE;
			break;

		case SHIFT_IDLE:
			if (ShiftHandler.request != REQUEST_EMPTY)
			{
				if (NULL != GearHandler.data)
				{
					ShiftHandler.state = SHIFT_PRELOAD;

					/* Save previous gear */
					GearHandler.prevGear = GearHandler.state;

					/* Set servo to change position */
					GearControl_RequestGearChange(&ShiftHandler.request, &savedState);

					/* Reset request and error, start WDG Timer */
					ShiftHandler.request = REQUEST_EMPTY;
					ShiftHandler.validation = VALIDATION_POLL;
					GearHandler.wdgStatus = TRUE;

					/* Clear flags */
					GearControl_SetError(GEAR_NOT_SHIFTED_ERROR | SHIFT_REQUEST_ERROR | TIMEOUT_ERROR, FALSE);
				}
			}
			else
			{
				if (TRUE == GearControl_PrepareCANMessage())
				{
					CAN_TransmitMsg(can_data);
				}
			}
			break;

		case SHIFT_PRELOAD:
			/* Wait for fuel cut */
			/* GEAR_CUT_DELAY * 4ms */
			if ((++fuelCutDelay) >= GEAR_CUT_DELAY)
			{
				fuelCutDelay = 0U;
				/* Trigger fuel cut */
				GearControl_FuelCutTrigger(ON);

				GearHandler.nextState = savedState;
				ShiftHandler.state = SHIFT_EXEC;
			}
			break;

		case SHIFT_DONE:
			GearControl_FuelCutTrigger(OFF);
			GearHandler.nextState = GEAR_INVALID;
			GearHandler.data = NULL;
			nextState = savedState;
			GearControl_WdgOff();
			ShiftControl_SetState(SHIFT_IDLE);
			MicroSwitch_PollAgain();
			break;

		case SHIFT_EXEC:
		default:
			/* In progress */
			break;
	}

	return nextState;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

void GearControl_SetControl(boolean control)
{
	GearHandler.activation = control;
}

boolean GearControl_IsActive(void)
{
	return GearHandler.activation;
}

ShiftStates ShiftControl_GetState(void)
{
	return ShiftHandler.state;
}

void ShiftControl_SetState(ShiftStates state)
{
	ShiftHandler.state = state;
}

void ShiftControl_SetValidation(boolean status)
{
	ShiftHandler.validation = status;
}
uint8 ShiftControl_GetValidation(void)
{
	return ShiftHandler.validation;
}
void GearControl_SetState(GearStates state)
{
	GearHandler.state = state;
}

GearStates GearControl_GetState(void)
{
	return GearHandler.state;
}

GearStates GearControl_GetNextState(void)
{
	return GearHandler.nextState;
}

void GearSensor_SetState(GearSensorStates state)
{
	GearHandler.GearSensState = state;
}

void ShiftControl_SetRequest(uint8 request)
{
	if (TRUE == GearControl_IsActive())
	{
		uint8 req = GearState_ValidateRequest(request);

		if (REQUEST_EMPTY != req)
		{
			/* Valid Request */
			ShiftHandler.request = req;
			GearHandler.data = &GearDataDecoder[(uint8)GearHandler.state];
		}
		else
		{
			GearControl_SetError(SHIFT_REQUEST_ERROR, TRUE);
		}
	}
	else
	{
		/* Gear Change disabled */
		GearControl_FuelCutTrigger(OFF);
		GearControl_SetServoPos(SET_DEG, POS_DEG_DEFAULT, 0);
		MicroSwitch_PollAgain();
	}
}

boolean GearControl_GetWdgStatus(void)
{
	return GearHandler.wdgStatus;
}

uint32 GearControl_GetErrors(void)
{
	return GearHandler.errors;
}

void GearControl_SetError(uint32 err, boolean set)
{
	if (TRUE == set)
	{
		GearHandler.errors |= err;
	}
	else
	{
		GearHandler.errors &= ~err;
	}
}

void GearControl_Init(__IO uint32* servoPWM)
{
	GearHandler.PWM = servoPWM;
}

/* Called in 4ms period */
void GearControl_WdgProcess(void)
{
	if (TRUE == GearHandler.wdgStatus)
	{
		if (GearHandler.wdgCnt < SHIFT_TIMEOUT)
		{
			++GearHandler.wdgCnt;
		}
		else
		{
			/* Gear not changed in 240ms after request */
			GearControl_SetServoPos(SET_DEG, POS_DEG_DEFAULT, 0);
			ShiftControl_SetState(SHIFT_DONE);
			GearControl_SetError(TIMEOUT_ERROR, TRUE);
			GearControl_WdgOff();
		}
	}
}

/* 500 - 0 deg */
/* 1500 - 90 deg */
/* 2500 - 180 deg */
void GearControl_SetServoPos(uint8 control, uint8 deg, uint32 PWM)
{
	switch (control)
	{
		case SET_PWM:
			/* Set PWM signal directly */
			if ((PWM >= POS_PWM_MIN) && (PWM <= POS_PWM_MAX))
			{
				*GearHandler.PWM = PWM;
				GearHandler.PwmRequest = PWM;
			}
			else
			{
				/* Error */
				GearControl_SetError(POSITION_REQUEST_ERROR, TRUE);
			}
			break;

		case SET_DEG:
			/* Set PWM signal by degrees conversion */
			if ((deg >= POS_DEG_MIN) && (deg <= POS_DEG_MAX))
			{
				if (deg < 90U)
				{
					/* 0-90 deg use floor */
					*GearHandler.PWM = (uint32)(floor((11.111111f * deg))) + (uint32)500U;
				}
				else
				{
					/* 90-180 deg use ceil */
					*GearHandler.PWM = (uint32)(ceil((11.111111f * deg))) + (uint32)500U;
				}
				GearHandler.PwmRequest = *GearHandler.PWM;
			}
			else
			{
				/* Error */
				GearControl_SetError(POSITION_REQUEST_ERROR, TRUE);
			}
			break;

		default:
			/* Error */
			GearControl_SetError(POSITION_REQUEST_ERROR, TRUE);
			break;
	}
}

/* Gear control main process */
void GearControl_Process(void)
{
	if (GEAR_INIT == GearHandler.state)
	{
		GearHandler.state = GearState_Init();
	}
	else
	{
		GearHandler.state = GearStateHandler();

		GearControl_WdgProcess();
	}
}

void GearControl_FuelCutTrigger(uint8 onOff)
{
	if (onOff == ON)
	{
		GearHandler.cutStatus = TRUE;
		HAL_GPIO_WritePin(FUEL_CUT_GPIO_Port, FUEL_CUT_Pin, GPIO_PIN_SET);
	}
	else
	{
		GearHandler.cutStatus = FALSE;
		HAL_GPIO_WritePin(FUEL_CUT_GPIO_Port, FUEL_CUT_Pin, GPIO_PIN_RESET);
	}
}

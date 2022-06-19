/*
 * GearSensor.c
 *
 *  Created on: 22.05.2021
 *      Author: Patryk Wittbrodt
 */

#include "GearSensor.h"

#include "main.h"


/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
#define N_CHANNELS (4U)

typedef struct
{
	const GearStates state;
	const uint16 adcVal;
	const uint16 adcThreshold;
	__IO uint8 validCnt;
}GearInfoS;

static GearInfoS GearInfo[] = {{GEAR_INIT   ,0xFFFFU,0U,   0},
							   {GEAR_1      ,2690U  ,80U,  0},
							   {GEAR_N      ,4000U  ,200U, 0},
							   {GEAR_2      ,1938U  ,80U,  0},
							   {GEAR_3      ,1390U  ,80U,  0},
							   {GEAR_4      ,888U   ,80U,  0},
							   {GEAR_5      ,518U   ,80U,  0},
							   {GEAR_6      ,190U   ,80U,  0},
							   {GEAR_IN_PROG,0xFFFFU,0U,   0},
							   {GEAR_INVALID,0      ,0U,   0}};

static GearStates state;

GearStates GearSensor_GetState(void)
{
	return state;
}

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */

static inline uint8 InRange(GearInfoS* gearinfo, uint8 nSamples);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
#if 0
/* Check if adc reading is within threshold for given gear position */
static inline uint8 InRange(GearInfoS* gearinfo, uint8 nSamples)
{
	uint8 in_range = FALSE;

	if ((gear_sensor.rawVal[0] != GEARSENSOR_UNDEFINED) &&
		(gearinfo->adcVal >= (gear_sensor.rawVal[0] - gearinfo->adcThreshold)) &&
		(gearinfo->adcVal <= (gear_sensor.rawVal[0] + gearinfo->adcThreshold)))
	{
		if (gearinfo->validCnt >= nSamples)
		{
			in_range = TRUE;
			gearinfo->validCnt = 0;
		}
		else
		{
			in_range = CHECK_IN_PROG;
			gearinfo->validCnt++;
		}
	}
	else
	{
		/* Reset valid counter and return FALSE */
		gearinfo->validCnt = 0;
	}

	return in_range;
}
#endif
/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

/* Get current gear by sensor reading */
GearStates GearSensor_GetStateBySensorAdc(uint8 nSamples)
{
	uint8 i = (uint8)GEAR_1; /* Start with GEAR 1 id */
	GearStates state = GEAR_INVALID;

	while (GearInfo[i].state != GEAR_INVALID)
	{
		uint8 status = InRange(&GearInfo[i], nSamples);

		if (status == TRUE)
		{
			state = GearInfo[i].state;
			break;
		}
		else if (status == CHECK_IN_PROG)
		{
			state = GEAR_IN_PROG;
			break;
		}
		++i;
	}

	return state;
}

#if 0
/* Check if gear was changed by monitoring IDLE state */
void GearSensor_Process(void)
{
	GearStates nextstate = GearControl_GetNextState();
	GearStates currentstate = GearControl_GetState();

	if ((SHIFT_IDLE == ShiftControl_GetState()) && (nextstate == GEAR_INVALID))
	{
		uint8 isInRange = InRange(&GearInfo[(uint8)currentstate], SAMPLES_3);

		if (FALSE == isInRange)
		{
			GearStates newState = GearSensor_GetStateBySensorAdc(SAMPLES_3);

			if (newState == GEAR_IN_PROG)
			{
				/* CHECK IN PROGRESS */
			}
			else
			{
				/* Gear found or unkown (adc == 0) */
				if (newState != GEAR_INVALID)
				{
					GearControl_SetState(newState);
					GearSensor_SetState(GEAR_NOT_CHANGED);
				}
				else
				{
					/* adc equal 0, unknown gear */
					GearSensor_SetState(GEAR_UNKNOWN);
				}

				ShiftControl_SetValidation(VALIDATION_DONE);
				GearControl_SetError(GEAR_NOT_SHIFTED_ERROR, TRUE);
			}
		}
		else if ((TRUE == isInRange) &&
				(VALIDATION_POLL == ShiftControl_GetValidation()))
		{
			/* Gear found after unkown sensor state */
			GearSensor_SetState(GEAR_OK);
			GearControl_SetError(GEAR_NOT_SHIFTED_ERROR, FALSE);
			ShiftControl_SetValidation(VALIDATION_DONE);
		}
		else
		{
			/* CHECK IN PROGRESS */
		}
	}
}

void GearSensor_DMAProcess(void)
{
	GearStates nextstate = GearControl_GetNextState();
	ShiftStates state = ShiftControl_GetState();

	testadcval = gear_sensor.rawVal[0];

	switch (state)
	{
		case SHIFT_EXEC:
			if ((GEAR_INVALID != nextstate) &&
				(TRUE == InRange(&GearInfo[(uint8)nextstate], N_SAMPLES)))
			{
				GearControl_SetServoPos(SET_DEG, POS_DEG_DEFAULT, 0);
				GearControl_SetError(GEAR_NOT_SHIFTED_ERROR, FALSE);
				ShiftControl_SetState(SHIFT_DONE);
			}
			break;

		case SHIFT_IDLE:
			if (GEARSENSOR_UNDEFINED == gear_sensor.rawVal[0])
			{
				GearControl_SetServoPos(SET_DEG, POS_DEG_DEFAULT, 0);
			}
			break;

		default:
			break;
	}
}
#endif

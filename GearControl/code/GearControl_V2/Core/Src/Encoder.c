/*
 * Encoder.c
 *
 *  Created on: 17.04.2021
 *      Author: Patryk Wittbrodt
 */

#if 0
#include "main.h"
#include "GearControl.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */

#define PARITY_BIT_ERROR (1u)
#define ANGULAR_POS_DATA_BITS (10u)
#define STATUS_BITS (4u)

static uint8 status_bits[4] = {0, 0, 0, 0};
static uint16 posBuff[4] = {0, 0, 0, 0};
static uint8 parity_bit = 0;
static uint16 start_pos = 0;
static uint16 encoder_pos = 0;

/* ---------------------------- */
/* Static function declarations */
/* ---------------------------- */

static uint16 Encoder_Read(void);
static uint16 Encoder_AvgSamples(uint16);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */

static uint16 Encoder_AvgSamples(uint16 new_sample)
{
	uint16 avg = 0;
	uint16 sample = 0;

	/* Queue new sample in FIFO */
	for(sample = 3; sample > 0U; --sample)
	{
		posBuff[sample] = posBuff[sample - 1];
		avg = (uint16)(avg + posBuff[sample]);
	}
	posBuff[0] = new_sample;
	avg += new_sample;
	avg /= 4U;

	return avg;
}

static uint16 Encoder_Read(void)
{
	uint16 pos = 0;
	uint8 i = 0;

	HAL_GPIO_WritePin(GPIOA, ABS_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, ABS_CS_Pin, GPIO_PIN_RESET);

	for(i = 0; i < ANGULAR_POS_DATA_BITS; ++i)
	{
		HAL_GPIO_WritePin(GPIOA, ABS_CLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, ABS_CLK_Pin, GPIO_PIN_SET);

		pos |= (uint16)(HAL_GPIO_ReadPin(GPIOA, ABS_DO_Pin) << (10u - (i + 1u)));
	}

	/* Status bits S1-S5 */
	for(i = 0; i < STATUS_BITS; ++i)
	{
		HAL_GPIO_WritePin(GPIOA, ABS_CLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, ABS_CLK_Pin, GPIO_PIN_SET);

		status_bits[i] = (uint8)HAL_GPIO_ReadPin(GPIOA, ABS_DO_Pin);
	}

	/* Parity bit P1 */
	HAL_GPIO_WritePin(GPIOA, ABS_CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, ABS_CLK_Pin, GPIO_PIN_SET);

	parity_bit |= (uint8)HAL_GPIO_ReadPin(GPIOA, ABS_DO_Pin);

	return pos;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

boolean Encoder_Init(void)
{
	static uint8 samples_i = 0;
	boolean ret = 0;
	uint16 val = 0;

	val = Encoder_Read();

	if ((val > 0) && (val < 1024U))
	{
		if (samples_i < 3)
		{
			posBuff[samples_i] = val;
			++samples_i;
		}
		else
		{
			start_pos = Encoder_AvgSamples(val);
			samples_i = 0;
			ret = TRUE;
		}
	}

	return ret;
}

void Encoder_Process(void)
{
	uint16 pos = Encoder_Read();
	encoder_pos = Encoder_AvgSamples(pos);
}
#endif

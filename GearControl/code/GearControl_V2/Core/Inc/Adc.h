/*
 * GearControl.h
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_ADC_INTERNAL_H_
#define INC_ADC_INTERNAL_H_

#include "main.h"

/*********** Macros ***********/
#define N_SAMPLES (4U)
#define N_CHANNELS (2U)

/*********** Data structures ***********/

typedef struct
{
	__IO uint16_t* rawVal;
	__IO uint16_t avg;
	__IO uint16_t avgBuff[N_SAMPLES];
} AdcDataChannel;

typedef enum
{
	/* ADC1 Channels */
	ADC_CHANNEL_GEAR_POS = 0U,
	ADC_CHANNEL_IS       = 1U,
	ADC_1_CHANNELS_COUNT = 2U,
	/* ADC2 Channels */
	ADC_CHANNEL_TPS_1    = 0U,
	ADC_CHANNEL_TPS_2    = 1U,
	ADC_CHANNEL_APPS_1   = 2U,
	ADC_CHANNEL_APPS_2   = 3U,
	ADC_2_CHANNELS_COUNT = 4U
} AdcChannelEnum;

typedef enum
{
	ADC_1_HANDLE = 0U,
	ADC_2_HANDLE = 1U,
	ADC_HANDLE_COUNT = 2U
} AdcHandleEnum;

static inline uint16 ADC_AvgSamples(__IO uint16_t* adcBuff, __IO uint16* new_sample)
{
	uint16 avg = 0U;
	uint16 sample = 0U;

	/* Queue new sample in FIFO */
	for(sample = 3U; sample > 0U; --sample)
	{
		adcBuff[sample] = adcBuff[sample - 1U];
		avg = (avg + adcBuff[sample]);
	}
	adcBuff[0U] = *new_sample;
	avg += *new_sample;
	avg /= 4U;

	return avg;
}

__IO AdcDataChannel* ADC_getAdcStruct(const AdcHandleEnum adcHandle);
__IO AdcDataChannel* ADC_getAdcChannelPtr(const AdcHandleEnum adcHandle, const AdcChannelEnum channel);
ErrorFlagsEnum ADC_Init(ADC_HandleTypeDef* adcHandle, const AdcHandleEnum handleId, const uint32 conversions);

#endif /* INC_ADC_INTERNAL_H_ */

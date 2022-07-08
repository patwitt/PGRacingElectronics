/*
 * GearControl.h
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_ADC_INTERNAL_H_
#define INC_ADC_INTERNAL_H_

#include "Types.h"
#include "stm32f4xx_hal.h"
#include "DefineConfig.h"
#include "Utils.h"
/*********** Macros ***********/

#define FIFO_START (5U)
#define N_CHANNELS (2U)

/*********** Data structures ***********/

typedef struct
{
	__IO uint16_t* raw;
	__IO AvgBuffer_U16 avgData;
#if CONFIG_ADC_SHOW_MIN_MAX
	uint16_t max;
	uint16_t min;
#endif
} AdcDataChannel;

typedef enum
{
	/* ADC1 Channels */
	ADC_CHANNEL_GEAR_SENS = 0U,
	ADC_CHANNEL_IS        = 1U,
	ADC_1_CHANNELS_COUNT  = 2U,
	/* ADC2 Channels */
	ADC_CHANNEL_TPS_1     = 0U,
	ADC_CHANNEL_TPS_2     = 1U,
	ADC_CHANNEL_APPS_1    = 2U,
	ADC_CHANNEL_APPS_2    = 3U,
	ADC_2_CHANNELS_COUNT  = 4U
} AdcChannelEnum;

typedef enum
{
	ADC_1_HANDLE = 0U,
	ADC_2_HANDLE = 1U,
	ADC_HANDLE_COUNT = 2U
} AdcHandleEnum;

AdcDataChannel* ADC_getAdcStruct(const AdcHandleEnum adcHandle);
AdcDataChannel* ADC_getAdcChannelPtr(const AdcHandleEnum adcHandle, const AdcChannelEnum channel);
ErrorEnum ADC_Init(ADC_HandleTypeDef* adcHandle, const AdcHandleEnum handleId, const uint32 conversions);

#endif /* INC_ADC_INTERNAL_H_ */

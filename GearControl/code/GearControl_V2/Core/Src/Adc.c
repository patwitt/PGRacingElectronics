/*
 * Adc.c
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "Adc.h"
#include "main.h"
#include "GearControl.h"
#include "stm32f4xx_hal_adc.h"

/* ---------------------------- */
/*          Local data          */
/* ---------------------------- */
/* Conversion limits */
static const AdcChannelEnum adcChannelsMaxConversions[ADC_HANDLE_COUNT] = {ADC_1_CHANNELS_COUNT, ADC_2_CHANNELS_COUNT};

/* DMA buffers */
static __IO uint16 adc1DmaBuff[ADC_1_CHANNELS_COUNT];
static __IO uint16 adc2DmaBuff[ADC_2_CHANNELS_COUNT];
static __IO uint16 *const adcDmaBuffers[ADC_HANDLE_COUNT] = {&adc1DmaBuff[0U], &adc2DmaBuff[0U]};

/* Adc data available for other IEs */
static AdcDataChannel adc1Data[ADC_1_CHANNELS_COUNT];
static AdcDataChannel adc2Data[ADC_2_CHANNELS_COUNT];
static AdcDataChannel *const adcData[ADC_HANDLE_COUNT] = {&adc1Data[0U], &adc2Data[0U]};

__IO AdcDataChannel* ADC_getAdcStruct(const AdcHandleEnum adcHandle)
{
	__IO AdcDataChannel* dmaAdc = NULL;

	if (adcHandle < ADC_HANDLE_COUNT)
	{
		dmaAdc = adcData[adcHandle];
	}

	return dmaAdc;
}

__IO AdcDataChannel* ADC_getAdcChannelPtr(const AdcHandleEnum adcHandle, const AdcChannelEnum channel)
{
	__IO AdcDataChannel* adcChannel = NULL;

	if ((adcHandle < ADC_HANDLE_COUNT) && (channel <= adcChannelsMaxConversions[adcHandle]))
	{
		adcChannel = &adcData[adcHandle][channel];
	}

	return adcChannel;
}

ErrorFlagsEnum ADC_Init(ADC_HandleTypeDef* adcHalHandle, const AdcHandleEnum adcHandle, const uint32 conversions)
{
	ErrorFlagsEnum err = ERROR_OK;

	if ((adcHandle < ADC_HANDLE_COUNT) && ((AdcChannelEnum)conversions <= adcChannelsMaxConversions[adcHandle]))
	{
		if (HAL_OK == HAL_ADC_Start_DMA(adcHalHandle, (uint32*)adcDmaBuffers[adcHandle], conversions))
		{
			for (uint32_t channel = 0U; channel < conversions; ++channel) {
				adcData[adcHandle][channel].rawVal = &adcDmaBuffers[adcHandle][channel];
			}
		} else {
			err = ERROR_HAL;
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

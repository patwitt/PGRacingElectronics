/*
 * Adc.c
 *
 *  Created on: 05.06.2022
 *      Author: Patryk Wittbrodt
 */

#include "Adc.h"
#include "main.h"
#include "DefineConfig.h"
#include "Utils.h"

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
static AdcDataChannel adc1Data[ADC_1_CHANNELS_COUNT] = {
		[ADC_CHANNEL_GEAR_SENS] = {.avgData.nSamples = N_SAMPLES_U16},
		[ADC_CHANNEL_IS]        = {.avgData.nSamples = N_SAMPLES_U16}
};
static AdcDataChannel adc2Data[ADC_2_CHANNELS_COUNT] = {
		[ADC_CHANNEL_TPS_1] = {.avgData.nSamples = N_SAMPLES_U16},
		[ADC_CHANNEL_TPS_2] = {.avgData.nSamples = N_SAMPLES_U16},
		[ADC_CHANNEL_APPS_1] = {.avgData.nSamples = N_SAMPLES_U16},
		[ADC_CHANNEL_APPS_2] = {.avgData.nSamples = N_SAMPLES_U16}
};
static AdcDataChannel *const adcData[ADC_HANDLE_COUNT] = {&adc1Data[0U], &adc2Data[0U]};

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
/**
 * @brief Initialization of the ADC module.
 * 
 * It starts the ADC and DMA, and initializes the ADC data structure.
 * 
 * @param adcHalHandle This is the HAL handle for the ADC peripheral.
 * @param adcHandle    The handle of the ADC to initialize.
 * @param conversions  The number of conversions to be done.
 * 
 * @return an error code.
 */
ErrorEnum ADC_Init(ADC_HandleTypeDef* adcHalHandle, const AdcHandleEnum adcHandle, const uint32 conversions)
{
	ErrorEnum err = ERROR_OK;

	if ((adcHandle < ADC_HANDLE_COUNT) && ((AdcChannelEnum)conversions <= adcChannelsMaxConversions[adcHandle]))
	{
		if (HAL_OK == HAL_ADC_Start_DMA(adcHalHandle, (uint32*)adcDmaBuffers[adcHandle], conversions))
		{
			for (uint32_t channel = 0U; channel < conversions; ++channel) {
				adcData[adcHandle][channel].raw = &adcDmaBuffers[adcHandle][channel];

#if CONFIG_ADC_SHOW_MIN_MAX
				adcData[adcHandle][channel].max = 0U;
				adcData[adcHandle][channel].min = 0xFFFFU;
#endif
			}
		} else {
			err = ERROR_HAL;
		}
	} else {
		err = ERROR_OOR;
	}

	return err;
}

/**
 * @brief Get ADC struct pointer.
 * 
 * @param adcHandle The ADC handle.
 * 
 * @return A pointer to the AdcDataChannel struct.
 */
AdcDataChannel* ADC_getAdcStruct(const AdcHandleEnum adcHandle)
{
	AdcDataChannel* dmaAdc = NULL;

	if (adcHandle < ADC_HANDLE_COUNT) {
		dmaAdc = adcData[adcHandle];
	}

	return dmaAdc;
}

/**
 * @brief Get ADC channel pointer.
 * 
 * If the ADC handle and channel are valid, return a pointer to the ADC channel data structure.
 *
 * @param adcHandle The ADC handle.
 * @param channel   The channel number of the ADC.
 * 
 * @return A pointer to the ADC channel.
 */
AdcDataChannel* ADC_getAdcChannelPtr(const AdcHandleEnum adcHandle, const AdcChannelEnum channel)
{
	AdcDataChannel* adcChannel = NULL;

	if ((adcHandle < ADC_HANDLE_COUNT) && (channel <= adcChannelsMaxConversions[adcHandle])) {
		adcChannel = &adcData[adcHandle][channel];
	}

	return adcChannel;
}

/*
 * RCFilter.c
 *
 *  Created on: 01.07.2022
 *      Author: Patryk Wittbrodt
 */

#include "RCFilter.h"
#include "Utils.h"

/* ---------------------------- */
/*          Constants           */
/* ---------------------------- */
#define PI_X_2 (6.28318530718f)

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

/**
 * @brief Initialization of the RC filter coefficients and the filter output.
 * 
 * @param filter       Pointer to the RCFilter structure.
 * @param cutoffFreqHz The cutoff frequency of the filter in Hz.
 * @param sampleTimeS  The time between each sample.
 * 
 * @return an error code.
 */
ErrorEnum RCFilter_Init(RCFilter *const filter, const float cutoffFreqHz, const float sampleTimeS)
{
	ErrorEnum error = ERROR_OK;

	if (filter != NULL) {
		/* Compute equivalent 'RC' constant from cut-off frequency eq. 1 */
		/*                   1
		 *  f-3db =    --------------
		 *               2 * pi * RC
		 */
		const float RC = 1.0f / (PI_X_2 * cutoffFreqHz);

		/* Pre-compute filter coefficients for first-order LPF */
		/*                 T                             RC
		 * coeff[0] =  -----------     coeff[1] =  --------------
		 *               T + RC                        T + RC
		 */
		filter->coeff[0U] = sampleTimeS / (sampleTimeS + RC);
		filter->coeff[1U] = RC / (sampleTimeS + RC);

		filter->output[0U] = 0.0f;
		filter->output[1U] = 0.0f;
	} else {
		error = ERROR_NULL;
	}

	return error;
}

/**
 * @brief Update the RC filter output.
 * 
 * @param filter    Pointer to the RCFilter structure.
 * @param newSample The new sample to be filtered.
 * 
 * @return The filtered sample.
 */
float RCFilter_Update(RCFilter *const filter, const float newSample)
{
	/* Shift output samples */
	filter->output[1U] = filter->output[0U];

	/* Compute new output sample */
	/*               T                         RC
	 * Vout[n] = ----------- * Vin[n]  + -------------- * Vout[n - 1]
	 *             T + RC                    T + RC
	 */
	filter->output[0U] = filter->coeff[0U] * newSample + filter->coeff[1U] * filter->output[1U];

	/* Return filtered sample */
	return filter->output[0U];
}

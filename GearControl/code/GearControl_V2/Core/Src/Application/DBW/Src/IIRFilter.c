/*
 * IIRFilter.c
 *
 *  Created on: 01.07.2022
 *      Author: Patryk Wittbrodt
 */

#include "IIRFilter.h"
#include "Utils.h"

/* ---------------------------- */
/*          Constants           */
/* ---------------------------- */
#define ALPHA_MAX (1.0f)
#define ALPHA_MIN (0.0f)

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum IIRFilter_Init(IIRFilter *const filter, const float alpha)
{
	ErrorEnum error = ERROR_OK;

	if (filter != NULL) {
		if (alpha < ALPHA_MIN) {
			filter->alpha = ALPHA_MIN;
		} else if (alpha > ALPHA_MAX) {
			filter->alpha = ALPHA_MAX;
		} else {
			filter->alpha = alpha;
		}
		filter->output = 0.0f;
	} else {
		error = ERROR_NULL;
	}

	return error;
}

float IIRFilter_Update(IIRFilter *const filter, const float newSample)
{
	filter->output = (ALPHA_MAX - filter->alpha) * newSample + filter->alpha * filter->output;

	return filter->output;
}

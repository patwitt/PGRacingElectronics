/*
 * RCFilter.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_MIDDLEWARE_FILTERING_SRC_RC_FILTER_H_
#define SRC_MIDDLEWARE_FILTERING_SRC_RC_FILTER_H_

#include "Types.h"

typedef struct {
	float coeff[2U];
	float output[2U];
	const float cutOffFreqHz;
	const float sampleTimeS;
} RCFilter;

ErrorEnum RCFilter_Init(RCFilter *const filter, const float cutoffFreqHz, const float sampleTimeS);
float RCFilter_Update(RCFilter *const filter, const float newSample);

#endif /* SRC_MIDDLEWARE_FILTERING_SRC_RC_FILTER_H_ */

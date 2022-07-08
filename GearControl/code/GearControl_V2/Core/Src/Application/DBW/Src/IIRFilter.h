/*
 * IIRFilter.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef IIR_FILTER_H_
#define IIR_FILTER_H_

#include "Types.h"

typedef struct {
	float alpha;
	float output;
} IIRFilter;

ErrorEnum IIRFilter_Init(IIRFilter *const filter, const float alpha);
float IIRFilter_Update(IIRFilter *const filter, const float newSample);

#endif /* IIR_FILTER_H_ */

/*
 * Utils.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "Types.h"

/* Absolute float value */
#define ABS_F(val) ((val < 0.0f) ? (val * (-1.0f)) : (val))
#define CLAMP_MAX(val, max) ((val > max) ? max : val)
#define CLAMP_MIN(val, min) ((val < min) ? min : val)

#define N_SAMPLES_F   (8U)
#define N_SAMPLES_U16 (10U)

#define NULL_ERR_CHECK1(error, param1) \
	if (param1 == NULL) { \
		error = ERROR_NULL; \
	} \

#define NULL_ERR_CHECK2(error, param1, param2) \
	if ((param1 == NULL) || (param2 == NULL)) { \
		error = ERROR_NULL; \
	} \

#define NULL_ERR_CHECK3(error, param1, param2, param3) \
	if ((param1 == NULL) || (param2 == NULL) || (param3 == NULL)) { \
		error = ERROR_NULL; \
	} \

#define NULL_CHECK1(param1) (param1 != NULL)

#define NULL_CHECK2(param1, param2) (param1 != NULL) && (param2 != NULL)

#define NULL_CHECK3(param1, param2, param3) (param1 != NULL) && (param2 != NULL) && (param3 != NULL)

typedef enum {
	DEBOUNCE_EXCEEDED,
	DEBOUNCE_IN_PROGRESS,
	DEBOUNCE_CNT_ZERO,
	DEBOUNCE_STATUS_CNT
} UtilsDebounceStatus;

typedef struct {
	float x1;
	float x2;
	float y1;
	float y2;
	float val;
} Slope;

typedef struct {
	float avg;
	float avgBuff[N_SAMPLES_F];
	float sum;
	uint32_t index;
	const uint32_t nSamples;
} AvgBuffer_F;

typedef struct {
	uint16_t avg;
	uint16_t avgBuff[N_SAMPLES_U16];
	uint16_t sum;
	uint32_t index;
	const uint32_t nSamples;
} AvgBuffer_U16;

typedef struct {
    const uint32_t x_length;
    const float *const x_values;
    const float *const y_values;
} table_1d;

static inline uint32_t Utils_Debounce(const bool_t failCondition, __IO uint32_t *const debCnt, const uint32_t debMs)
{
	UtilsDebounceStatus debounce = DEBOUNCE_IN_PROGRESS;

	if (failCondition) {
		if (*debCnt <= debMs) {
			++(*debCnt);
		}
	} else {
		if (*debCnt > 0U) {
			--(*debCnt);
		}
	}

	if (*debCnt > debMs) {
		debounce = DEBOUNCE_EXCEEDED;
	} else if (*debCnt == 0U) {
		debounce = DEBOUNCE_CNT_ZERO;
	} else { /* Do nothing, in progress */}

	return debounce;
}

static inline float Utils_CalcSlope(Slope *const slope, const float div)
{
    return (slope->y2 - slope->y1) / div;
}

static inline void Utils_RollingAverage_U16(__IO AvgBuffer_U16 *const avgData, const uint16_t newSample)
{
	if NULL_CHECK1(avgData) {
		/* Store new sample in buffer */
		avgData->avgBuff[avgData->index] = newSample;

		/* Add newest sample */
		avgData->sum += newSample;

		/* Calculate new index */
		avgData->index = (avgData->index + 1U) % avgData->nSamples;

		/* Delete oldest sample */
		avgData->sum -= avgData->avgBuff[avgData->index];

		/* Calculate average */
		avgData->avg = avgData->sum / (avgData->nSamples - 1U);
	}
}

static inline void Utils_RollingAverage_F(__IO AvgBuffer_F *const avgData, const float newSample)
{
	/* Store new sample in buffer */
	avgData->avgBuff[avgData->index] = newSample;

	/* Add newest sample */
	avgData->sum += newSample;

	/* Calculate new index */
	avgData->index = (avgData->index + 1U) % avgData->nSamples;

	/* Delete oldest sample */
	avgData->sum -= avgData->avgBuff[avgData->index];

	/* Calculate average */
	avgData->avg = avgData->sum / (avgData->nSamples - 1U);
}

static __IO inline void Utils_UpdateMax_U16(const uint16 value, uint16 *const max)
{
	if (value > *max) {
		*max = value;
	}
}

static __IO inline void Utils_UpdateMinMax_F(const float value, float *const min, float *const max)
{
	if (value < *min) {
		*min = value;
	}
	if (value > *max) {
		*max = value;
	}
}

static __IO inline void Utils_UpdateMinMax_U16(const uint16 value, uint16 *const min, uint16 *const max)
{
	if (value < *min) {
		*min = value;
	}
	if (value > *max) {
		*max = value;
	}
}

/* https://www.avrfreaks.net/forum/implementing-look-tables-c */
/* https://www.electro-tech-online.com/threads/linear-interpolation-and-lookup-tables-c.147507/ */

/**
* Returns the interpolated y-value.
* Saturates to y0 or y1 if x outside interval [x0, x1].
*/
static inline float Utils_interpolateSegment(const float x0, const float y0, const float x1, const float y1, const float x)
{
    if (x <= x0) { return y0; }
    if (x >= x1) { return y1; }

    float interp1d = (x - x0) / (x1 - x0);

    return y0 + interp1d * (y1 - y0);
}
/* 1D Table lookup with interpolation */
static inline float Utils_interpolateTable1d(const table_1d *const LUT, const float interpolatedVal)
{
    /* Check input bounds and saturate if out-of-bounds */
    if (interpolatedVal >= LUT->x_values[LUT->x_length - 1U]) {
       /* x-value too large, saturate to max y-value */
        return LUT->y_values[LUT->x_length - 1U];
    }

    if (interpolatedVal <= LUT->x_values[0U]) {
       /* x-value too small, saturate to min y-value */
        return LUT->y_values[0U];
    }

    uint32_t segment = 1U;

    while ((segment < LUT->x_length) && (interpolatedVal >= LUT->x_values[segment])) {
    	++segment;
    }

    return (segment < LUT->x_length) ? Utils_interpolateSegment(LUT->x_values[segment],   /* x0 */
    						   	   	   	   	   	   	   	        LUT->y_values[segment],   /* y0 */
														        LUT->x_values[segment + 1U], /* x1 */
			                                                    LUT->y_values[segment + 1U], /* y1 */
																interpolatedVal) : LUT->y_values[LUT->x_length -1U];
}

#endif /* INC_UTILS_H_ */

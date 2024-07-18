/*
 * KalmanFilter.h
 *
 *  Created on: May 27, 2024
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_MIDDLEWARE_FILTERING_SRC_KALMANFILTER_H_
#define SRC_MIDDLEWARE_FILTERING_SRC_KALMANFILTER_H_

typedef struct {
	float ADC_OLD_Value;
	float P_k1_k1;
	float kalman_adc_old;
	float Kg;
	float P_k_k1;
	const float Q;
	const float R;
} KalmanFilter;

float KalmanFilter_Update(KalmanFilter* kalman, const float ADC_Value);

#endif /* SRC_MIDDLEWARE_FILTERING_SRC_KALMANFILTER_H_ */

/*
 * KalmanFilter.h
 *
 *  Created on: May 27, 2024
 *      Author: lorem
 */

#ifndef SRC_APPLICATION_DBW_SRC_KALMANFILTER_H_
#define SRC_APPLICATION_DBW_SRC_KALMANFILTER_H_

typedef struct {
	float ADC_OLD_Value;
	float P_k1_k1;
	float kalman_adc_old;
	float Kg;
	float P_k_k1;
} KalmanFilter;

float KalmanFilter_Update(KalmanFilter* kalman, const float ADC_Value);

#endif /* SRC_APPLICATION_DBW_SRC_KALMANFILTER_H_ */

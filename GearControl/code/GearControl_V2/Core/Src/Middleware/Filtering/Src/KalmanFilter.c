#include "KalmanFilter.h"


/*
 * Q: Regulation noise, Q increases, dynamic response becomes faster,
 * and convergence stability becomes worse
 *
 * R: Test noise, R increases, dynamic response becomes slower,
 * convergence stability becomes better
 *
 */
float KalmanFilter_Update(KalmanFilter* kalman, const float ADC_Value)
{
    float x_k1_k1;
    float x_k_k1;
    float kalman_adc;
    float Z_k = ADC_Value;

    x_k1_k1 = kalman->kalman_adc_old;

    x_k_k1 = x_k1_k1;
    kalman->P_k_k1 = kalman->P_k1_k1 + kalman->Q;

    kalman->Kg = kalman->P_k_k1 / (kalman->P_k_k1 + kalman->R);

    kalman_adc = x_k_k1 + kalman->Kg * (Z_k - kalman->kalman_adc_old);
    kalman->P_k1_k1 = (1.0f - kalman->Kg) * kalman->P_k_k1;
    kalman->P_k_k1 = kalman->P_k1_k1;

    kalman->ADC_OLD_Value = ADC_Value;
    kalman->kalman_adc_old = kalman_adc;

    return kalman_adc;
}

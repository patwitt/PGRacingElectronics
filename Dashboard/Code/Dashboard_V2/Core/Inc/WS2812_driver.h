#ifndef WS2812_DRIVER_H_
#define WS2812_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
 
#include "stm32f7xx.h"
#define GEAR_NUMBER 6
#define MAX_ENGINE_TORQUE 70
#define USE_OPTIMAL_SHIFT_RPM
//#define MY_OPTIMAL_SHIFT_IMPLEMENTATION


#define BIT_0_TIME		43
#define BIT_1_TIME		86

#define RESET_LEN		40
#define LED_NUMBER		15



void WS2812_Init();

void setLed(uint8_t ledNumber, uint8_t red, uint8_t green, uint8_t blue);
void updateLeds(uint16_t rpm, uint8_t mode);
void ComputeOptimalPoints();

#ifdef __cplusplus
}
#endif

#endif

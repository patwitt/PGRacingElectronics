#ifndef WS2812_DRIVER_H_
#define WS2812_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
 
#include "stm32f7xx.h"
 
void WS2812_Init();

void setLed(uint8_t ledNumber, uint8_t red, uint8_t green, uint8_t blue);
void updateLeds(int rpm, int mode);
void ComputeOptimalPoints();

#ifdef __cplusplus
}
#endif

#endif

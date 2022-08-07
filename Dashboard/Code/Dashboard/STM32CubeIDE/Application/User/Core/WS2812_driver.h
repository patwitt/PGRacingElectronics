#ifndef WS2812_DRIVER_H_
#define WS2812_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
 
#include "stm32f7xx.h"
 
void leds_init();

void set_green(int num, bool light);
void set_red(int num, bool light);
void set_purple(int num, bool light);

void set_all();
void clear_all();
void light_leds(int rpm, int mode);

void set_leds(int rpm, int lowest);
void set_one_green(int i);
void set_one_red(int i);
void set_one_purple(int i);
void clear_one(int i);

#ifdef __cplusplus
}
#endif

#endif

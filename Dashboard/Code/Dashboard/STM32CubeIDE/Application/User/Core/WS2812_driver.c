#include "tim.h"
#include "math.h"
#include <stdbool.h>
#include "WS2812_driver.h"


//extern TIM_HandleTypeDef htim3;
//extern DMA_HandleTypeDef hdma_tim3_ch1_trig;

#define BIT_0_TIME		43
#define BIT_1_TIME		86

#define RESET_LEN		40
#define LED_N			15

volatile uint16_t leds[RESET_LEN + 24 * LED_N];

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
}

void leds_init(void)
{
	HAL_StatusTypeDef status;
  int i;
  for (i = 0; i < 24 * LED_N; i++)
	  leds[i] = BIT_0_TIME;

  for (i = 0; i < RESET_LEN; i++)
	  leds[i + LED_N * 24] = 0;

 //status = HAL_TIM_Base_Start(&htim3);


  status  = HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, leds,RESET_LEN + 24 * LED_N);
  //HAL_TIMEx_PWMN_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, sizeof(leds)/sizeof(uint16_t));
  //HAL_Delay(1000);
}

void set_green(int num, bool light){
	for (int i = 0; i < num; i++){
		if (light == true) leds[i*24 + 2] = BIT_1_TIME;
		else leds[(4-i)*24 + 2] = BIT_0_TIME;
	}
}
void set_red(int num, bool light){
	for (int i = 0; i < num; i++){
		if (light == true) leds[(5+i)*24 + 10] = BIT_1_TIME;
		else leds[(9-i)*24 + 10] = BIT_0_TIME;
	}
}
void set_purple(int num, bool light){
	for (int i = 0; i < num; i++){
		if (light == true){
			leds[(10+i)*24 + 10] = BIT_1_TIME;
			leds[(10+i)*24 + 16] = BIT_1_TIME;
		} else {
			leds[(14-i)*24 + 10] = BIT_0_TIME;
			leds[(14-i)*24 + 16] = BIT_0_TIME;
		}
	}
}

void set_all(){
	set_green(5,true);
	set_red(5,true);
	set_purple(5,true);
}

void clear_all(){
	set_green(5,false);
	set_red(5,false);
	set_purple(5,false);
}

void set_one_green(int num){
	leds[RESET_LEN + num*24 + 4] = BIT_1_TIME;
}

void set_one_red(int num){
	leds[RESET_LEN + num*24 + 12] = BIT_1_TIME;
}

void set_one_purple(int num){
	leds[RESET_LEN + num*24 + 12] = BIT_1_TIME;
	leds[RESET_LEN + num*24 + 20] = BIT_1_TIME;
}

void clear_one(int num){
	leds[RESET_LEN + num*24 + 4] = BIT_0_TIME;
	leds[RESET_LEN + num*24 + 12] = BIT_0_TIME;
	leds[RESET_LEN + num*24 + 20] = BIT_0_TIME;
}

void set_leds(int rpm, int lowest){
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, sizeof(leds));
	int active = ceil((rpm-lowest) / (12000.0F - lowest) * LED_N);
	if (active < 0 ) active = 0;
	for (int i = 0; i < LED_N; i++){
		if (i < active){
			if (i < LED_N * 0.33){
				set_one_green(i);
			}
			else if (i < LED_N*0.66){
				set_one_red(i);
			}
			else {
				set_one_purple(i);
			}
		} else {
			clear_one(i);
		}
	}
}

void light_leds(int rpm, int type) {
	int how_many;
	switch (type) {
	/* ver 1 (8000-11500 rpm)
	 * 1-10 set linear than last 5 set on 11500 rpm
	 * */
	case 1:
		if (rpm <= 8000) {
			clear_all();
		}
		else {
			if (rpm <= 9500) {
				how_many = (9500 - rpm) / 300;
				// light
				set_green(5 - how_many, true);
				// clear
				set_green(how_many, false);
				set_red(5, false);
			}
			else if (rpm <= 11000) {
				how_many = (11000 - rpm) / 300;
				// light
				set_green(5, true);
				set_red(5 - how_many, true);
				// clear
				set_red(how_many, false);
				set_purple(5, false);
			}
			else if (rpm >= 11500) {
				set_all();
			}
		}
		break;
	case 2:
		// ver2 (0 - 11500 rpm) linear
		set_leds(rpm, 0.0F);
		break;
	case 3:
		// ver3 (8000 - 11500 rpm) linear
		set_leds(rpm, 8000.0F);
		break;
	}
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, &leds, RESET_LEN + 24 * LED_N);
}

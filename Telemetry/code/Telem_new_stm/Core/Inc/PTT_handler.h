/*
 * PTT_hander.h
 *
 *  Created on: 18 sie 2022
 *      Author: Bartosz Janicki
 */
#include "stm32l4xx_hal.h"


#ifndef __PTT_HANDLER_H__
#define __PTT_HANDLER_H__

extern int PTT_status;


int PTT_control(uint8_t data_frame[]);

#endif /* INC_PTT_HANDLER_H_ */

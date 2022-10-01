/*
 * PTT_handler.c
 *
 *  Created on: 18 sie 2022
 *      Author: Bartosz Janicki
 */
/*
 */
#include "PTT_handler.h"

#define ACTIVE_BIT 0xFF
#define INACTIVE_BIT 0x00

int PTT_status=0;

int PTT_control(uint8_t data_frame[])
{
	if(data_frame[0]==ACTIVE_BIT)
	{
		PTT_status=1;
	}
	else if(data_frame[1]==INACTIVE_BIT)
	{
		PTT_status=0;
	}
	return PTT_status;
}


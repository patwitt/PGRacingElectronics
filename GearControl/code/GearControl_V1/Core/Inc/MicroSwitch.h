/*
 * MicroSwitch.h
 *
 *  Created on: 24.04.2021
 *      Author: User
 */

#ifndef INC_MICROSWITCH_H_
#define INC_MICROSWITCH_H_

#define DEBOUNCE_10MS (5U)
#define DEBOUNCE_20MS (10U)
#define DEBOUNCE_50MS (25U)
#define BUTTON_PRESSED_ADC_THRESHOLD (3800U)
#define BUTTON_OFF_ADC_TRESHOLD (900U)
#define BUTTON_CNT    (2U)

typedef enum
{
	POLL_SHIFT_DOWN = 0U,
	POLL_SHIFT_UP = 1U,
	POLL_EXEC = 2U,
	POLL_LOW = 3U,
	POLL_NONE = 4U
}PollStates;

void MicroSwitch_PollStatus(void);
void MicroSwitch_PollAgain(void);
void MicroSwitch_Init(__IO uint16* const);
void MicroSwitch_Process(void);
void MicroSwitch_Start(void);
void MicroSwitch_Stop(void);
boolean MicroSwitch_GetStatus(void);
void OnOffSwitch_Process(void);
#endif /* INC_MICROSWITCH_H_ */

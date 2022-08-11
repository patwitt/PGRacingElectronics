/*
 * DisplaySetup.h
 *
 *  Created on: Aug 10, 2022
 *      Author: Marcin
 */

#ifndef APPLICATION_USER_CORE_DISPLAY_SETUP_H_
#define APPLICATION_USER_CORE_DISPLAY_SETUP_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
 uint8_t brightness; // display backlight brightness in %
 uint8_t ledBarMode;
}displaySetup_t;

displaySetup_t displaySetup = { 70 , 2 };

#ifdef __cplusplus
}
#endif




#endif /* APPLICATION_USER_CORE_DISPLAY_SETUP_H_ */

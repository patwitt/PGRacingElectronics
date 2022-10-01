/*
 * displaySetup.h
 *
 *  Created on: Aug 10, 2022
 *      Author: Marcin
 */

#ifndef APPLICATION_USER_CORE_DISPLAY_SETUP_H_
#define APPLICATION_USER_CORE_DISPLAY_SETUP_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	uint8_t brightness; // display backlight brightness in %
	uint8_t ledBarMode;
	uint8_t redLedBrightness;
	uint8_t greenLedBrightness;
	uint8_t blueLedBrightness;
} displaySetup_t;

enum DisplayCANFrames{
	GIT_BUILD_TAG = 0x100,
	GIT_COMMIT,
	BUILD_DATE

};

extern displaySetup_t displaySetup;

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_CORE_DISPLAY_SETUP_H_ */

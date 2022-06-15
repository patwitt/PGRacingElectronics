/**
  ******************************************************************************
  * File Name          : TouchGFXHAL.cpp
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include <TouchGFXHAL.hpp>

/* USER CODE BEGIN TouchGFXHAL.cpp */

//#include <TouchGFXGeneratedHAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
//#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include "stm32f7xx.h"
#include "stm32f7xx_hal_ltdc.h"
using namespace touchgfx;

namespace
{
    static uint16_t lcd_int_active_line;
    static uint16_t lcd_int_porch_line;
}

void TouchGFXHAL::initialize() {
	// Calling parent implementation of initialize().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.
	// Please note, HAL::initialize() must be called to initialize the framework.

	TouchGFXGeneratedHAL::initialize();
}

/**
 * Gets the frame buffer address used by the TFT controller.
 *
 * @return The address of the frame buffer currently being displayed on the TFT.
 */
uint16_t* TouchGFXHAL::getTFTFrameBuffer() const {
	// Calling parent implementation of getTFTFrameBuffer().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	return TouchGFXGeneratedHAL::getTFTFrameBuffer();
}

/**
 * Sets the frame buffer address used by the TFT controller.
 *
 * @param [in] address New frame buffer address.
 */
void TouchGFXHAL::setTFTFrameBuffer(uint16_t *address) {
	// Calling parent implementation of setTFTFrameBuffer(uint16_t* address).
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::setTFTFrameBuffer(address);
}

/**
 * This function is called whenever the framework has performed a partial draw.
 *
 * @param rect The area of the screen that has been drawn, expressed in absolute coordinates.
 *
 * @see flushFrameBuffer().
 */
void TouchGFXHAL::flushFrameBuffer(const touchgfx::Rect &rect) {
	// Calling parent implementation of flushFrameBuffer(const touchgfx::Rect& rect).
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.
	// Please note, HAL::flushFrameBuffer(const touchgfx::Rect& rect) must
	// be called to notify the touchgfx framework that flush has been performed.
	// To calculate he start adress of rect,
	// use advanceFrameBufferToRect(uint8_t* fbPtr, const touchgfx::Rect& rect)
	// defined in TouchGFXGeneratedHAL.cpp

	TouchGFXGeneratedHAL::flushFrameBuffer(rect);
}

bool TouchGFXHAL::blockCopy(void *RESTRICT dest, const void *RESTRICT src,
		uint32_t numBytes) {
	return TouchGFXGeneratedHAL::blockCopy(dest, src, numBytes);
}

/**
 * Configures the interrupts relevant for TouchGFX. This primarily entails setting
 * the interrupt priorities for the DMA and LCD interrupts.
 */
void TouchGFXHAL::configureInterrupts() {
	// Calling parent implementation of configureInterrupts().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::configureInterrupts();

	// custom code
    lcd_int_active_line = (LTDC->BPCR & 0x7FF) - 1;
    lcd_int_porch_line = (LTDC->AWCR & 0x7FF) - 1;
}

/**
 * Used for enabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::enableInterrupts() {
	// Calling parent implementation of enableInterrupts().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::enableInterrupts();
}

/**
 * Used for disabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::disableInterrupts() {
	// Calling parent implementation of disableInterrupts().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::disableInterrupts();
}

/**
 * Configure the LCD controller to fire interrupts at VSYNC. Called automatically
 * once TouchGFX initialization has completed.
 */
void TouchGFXHAL::enableLCDControllerInterrupt() {
	// Calling parent implementation of enableLCDControllerInterrupt().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::enableLCDControllerInterrupt();
}
/*
extern "C" {
void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc) {
	static uint8_t frameCounter = 5;
	if (LTDC->LIPCR == lcd_int_active_line) {
		//entering active area
		HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_porch_line);
		HAL::getInstance()->vSync();
		if(frameCounter == 50)
		{
		OSWrappers::signalVSync();
		frameCounter = 0;
		}
		else{
			frameCounter++;
		}
		// Swap frame buffers immediately instead of waiting for the task to be scheduled in.
		// Note: task will also swap when it wakes up, but that operation is guarded and will not have
		// any effect if already swapped.
		HAL::getInstance()->swapFrameBuffers();
		GPIO::set(GPIO::VSYNC_FREQ);
	} else {
		//exiting active area
		HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_active_line);
		GPIO::clear(GPIO::VSYNC_FREQ);
		HAL::getInstance()->frontPorchEntered();
	}
}
}*/
/* USER CODE END TouchGFXHAL.cpp */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

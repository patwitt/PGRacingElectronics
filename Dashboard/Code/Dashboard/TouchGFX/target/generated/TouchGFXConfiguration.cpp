/**
******************************************************************************
* File Name : TouchGFXConfiguration.cpp
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
#include <texts/TypedTextDatabase.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <BitmapDatabase.hpp>
#include <platform/driver/lcd/LCD32bpp.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
#include <touchgfx/hal/NoDMA.hpp>
#include <TouchGFXHAL.hpp>
#include <STM32TouchController.hpp>
#include <stm32f7xx_hal.h>

extern "C" void touchgfx_init();
extern "C" void touchgfx_taskEntry();

static STM32TouchController tc;
static NoDMA dma;
static LCD32bpp display;
static ApplicationFontProvider fontProvider;
static Texts texts;
static TouchGFXHAL hal(dma, display, tc, 800, 480);

void touchgfx_init()
{
    Bitmap::registerBitmapDatabase(BitmapDatabase::getInstance(), BitmapDatabase::getInstanceSize());
    TypedText::registerTexts(&texts);
    Texts::setLanguage(0);

    FontManager::setFontProvider(&fontProvider);

    FrontendHeap& heap = FrontendHeap::getInstance();
    /*
     * we need to obtain the reference above to initialize the frontend heap.
     */
    (void)heap;

    /*
     * Initialize TouchGFX
     */
    hal.initialize();
}

void touchgfx_taskEntry()
{
    /*
     * Main event loop will check for VSYNC signal, and then process next frame.
     *
     * Note This function returns immediately if there is no VSYNC signal.
     */
     if (OSWrappers::isVSyncAvailable())
     {
         hal.backPorchExited();
     }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

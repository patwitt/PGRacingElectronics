/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/start_screen/startViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"

startViewBase::startViewBase()
{

    __background.setPosition(0, 0, 800, 480);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));

    box1.setPosition(0, 0, 800, 480);
    box1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));

    scalableImage1.setBitmap(touchgfx::Bitmap(BITMAP_PGRACINGTEAMLOGO_ID));
    scalableImage1.setPosition(100, -45, 580, 580);
    scalableImage1.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);

    add(__background);
    add(box1);
    add(scalableImage1);
}

void startViewBase::setupScreen()
{

}

#ifndef TEMPBOX_HPP
#define TEMPBOX_HPP

#include "ColorHelper.hpp"
#include <gui_generated/containers/tempBoxBase.hpp>

#define LINES_NUM 220

class tempBox : public tempBoxBase
{
public:
    tempBox();
    virtual ~tempBox() {}

    virtual void initialize();

    int tickCounter = 0;

    void debugColor();
    void redraw();

    int tempValues[220];
    /* If less or more than 220 interpolate */
    int numberOfValues = 220;

protected:

    touchgfx::Line lines[220];
    touchgfx::PainterRGB888 linesPainter[220];
private:
    
    colortype mapTemp(float inTemp);
};

#endif // TEMPBOX_HPP

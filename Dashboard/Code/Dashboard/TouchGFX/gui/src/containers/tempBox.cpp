#include <gui/containers/tempBox.hpp>
#include <cmath>

tempBox::tempBox()
{
    for(int i=0; i < 220; i++)
    {
        lines[i].setPosition(0,0,220,60);
        linesPainter[i].setColor(touchgfx::Color::getColorFromRGB(10 ,255, 0));
        lines[i].setPainter(linesPainter[i]);
        lines[i].setStart(i,0);
        lines[i].setEnd(i,60);
        lines[i].setLineWidth(1);
        lines[i].setLineEndingStyle(touchgfx::Line::ROUND_CAP_ENDING);
        add(lines[i]);
    }

}

void tempBox::initialize()
{
    tempBoxBase::initialize();
    tempValues[0] = 25;
    tempValues[1] = 25;
    tempValues[2] = 25;
    tempValues[3] = 25;
    tempValues[4] = 25;
    tempValues[5]= 25;
    numberOfValues = 5;
}


void tempBox::debugColor()
{
    tickCounter++;
    for(int i=0; i < 220; i++)
    {
        linesPainter[i].setColor(touchgfx::Color::getColorFromRGB((tickCounter + i)% 255 , (255 - tickCounter + i) % 255, 0));
    }
}
 
colortype tempBox::mapTemp(float inTemp)
{
    /* Upper scale temperature */
    const float maxTemp = 200; 
    const float p = inTemp/maxTemp;
    const colortype lowerTempColor = colors::green;
    const colortype upperTempColor = colors::red;

    return lerpColor(lowerTempColor, upperTempColor, p);
}

void tempBox::redraw()
{
    for( int i = 0; i < LINES_NUM; i++)
    {
        float p = (float)i / LINES_NUM;
        float iInRange = p * numberOfValues;

        if(iInRange < 0)
            iInRange = 0;
        else if(iInRange >= numberOfValues)
            iInRange = numberOfValues - 1;

        //if iInRange is abs value get data from array, else liniear interpolate between values
        int kCeil = ceilf(iInRange);
        int kFloor = floorf(iInRange);

        float alpha = iInRange - kFloor;
        float endValue = lerp(tempValues[kFloor], tempValues[kCeil], alpha);
        
        const colortype tempColor = mapTemp(endValue);
        linesPainter[i].setColor(tempColor);
    }
}

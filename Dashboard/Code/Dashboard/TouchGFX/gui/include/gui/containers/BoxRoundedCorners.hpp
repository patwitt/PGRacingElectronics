/**
 * @file BoxRoundedCorners.hpp
 * @author cameronf
 * @cameron.j.fyfe@gmail.com
 * @brief 
 * @date 2019-04-25
 * 
 * This code was originally modified from code provided as part of the TouchGFX 4.10.0 distribution.
 * 
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 */
#ifndef BOX_ROUNDED_CORNERS_HPP_
#define BOX_ROUNDED_CORNERS_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>


namespace touchgfx {


class BoxRoundedCorners : public Container
{
public:
    
    BoxRoundedCorners()
    {
        // Default pixel corner radius
        setCornerRadius(10);

        circleTL.setLineWidth(0);
        circleTL.setArc(270, 360);
        circleTL.setPainter(circlePainter);

        circleTR.setLineWidth(0);
        circleTR.setArc(0, 90);
        circleTR.setPainter(circlePainter);

        circleBR.setLineWidth(0);
        circleBR.setArc(90, 180);
        circleBR.setPainter(circlePainter);

        circleBL.setLineWidth(0);
        circleBL.setArc(180, 270);
        circleBL.setPainter(circlePainter);

        add(boxCenter);
        add(boxLeft);
        add(boxRight);

        add(circleTL);
        add(circleTR);
        add(circleBR);
        add(circleBL);
    }

    virtual ~BoxRoundedCorners() {}

    void setCornerRadius(int16_t r)
    {
        cornerRadius = r;
        updateDimensions();
    }

    virtual void setX(int16_t x)
    {
        Container::setX(x);
    }

    virtual void setY(int16_t y)
    {
        Container::setY(y);
    }

    virtual void setWidth(int16_t width)
    {
        Container::setWidth(width);
        updateDimensions();
    }

    virtual void setHeight(int16_t height)
    {
        Container::setHeight(height);
        updateDimensions();
    }

    virtual void setPosition(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        Container::setXY(x, y);
        Container::setWidth(width);
        Container::setHeight(height);
        updateDimensions();
    }

    void setColor(colortype color)
    {
        this->color = color;
        boxCenter.setColor(color);
        boxLeft.setColor(color);
        boxRight.setColor(color);
        circlePainter.setColor(color);
    }

    colortype getColor() const
    {
        return this->color;
    }

    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
        boxCenter.setAlpha(alpha);
        boxLeft.setAlpha(alpha);
        boxRight.setAlpha(alpha);
        circleTL.setAlpha(alpha);
        circleTR.setAlpha(alpha);
        circleBR.setAlpha(alpha);
        circleBL.setAlpha(alpha);        
    }

    uint8_t getAlpha() const
    {
        return this->alpha;
    }

protected:

    void updateDimensions()
    {
        boxCenter.setXY(cornerRadius, 0);
        boxCenter.setWidth(rect.width-2*cornerRadius);
        boxCenter.setHeight(rect.height);

        boxLeft.setXY(0, cornerRadius);
        boxLeft.setWidth(cornerRadius);
        boxLeft.setHeight(rect.height-2*cornerRadius);

        boxRight.setXY(rect.width-cornerRadius, cornerRadius);
        boxRight.setWidth(cornerRadius);
        boxRight.setHeight(rect.height-2*cornerRadius);

        circleTL.setXY(0, 0);
        circleTL.setCenter(cornerRadius, cornerRadius);

        circleTR.setXY(rect.width-cornerRadius, 0);
        circleTR.setCenter(0, cornerRadius);

        circleBR.setXY(rect.width-cornerRadius, rect.height-cornerRadius);
        circleBR.setCenter(0, 0);

        circleBL.setXY(0, rect.height-cornerRadius);
        circleBL.setCenter(cornerRadius, 0);

        circleTL.setWidth(cornerRadius);
        circleTL.setHeight(cornerRadius);
        circleTL.setRadius(cornerRadius);
        circleTR.setWidth(cornerRadius);
        circleTR.setHeight(cornerRadius);
        circleTR.setRadius(cornerRadius);
        circleBR.setWidth(cornerRadius);
        circleBR.setHeight(cornerRadius);
        circleBR.setRadius(cornerRadius);
        circleBL.setWidth(cornerRadius);
        circleBL.setHeight(cornerRadius);
        circleBL.setRadius(cornerRadius);
    }

    int16_t cornerRadius;
    colortype color;
    uint8_t alpha;

    // Use 3 boxes intead of overlapping boxes so transparency works
    Box boxCenter;
    Box boxLeft;
    Box boxRight;

    Circle circleTL;
    Circle circleTR;
    Circle circleBR;
    Circle circleBL;
    PainterRGB565 circlePainter;

private:

};


} // namespace touchgfx


#endif // BOX_ROUNDED_CORNERS_HPP_

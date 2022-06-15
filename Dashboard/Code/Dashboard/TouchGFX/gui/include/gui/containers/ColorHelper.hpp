#pragma once
#include <touchgfx/Color.hpp>
#include <math.h>

struct color_t
{
    int r;
    int g;
    int b;
};

namespace colors
{
    using namespace touchgfx;
    const colortype black = Color::getColorFrom24BitRGB(0, 0, 0);
    const colortype red = Color::getColorFrom24BitRGB(255, 0, 0);
    const colortype green = Color::getColorFrom24BitRGB(0, 255, 0);
    const colortype blue = Color::getColorFrom24BitRGB(0, 0, 255);
    const colortype orange = Color::getColorFrom24BitRGB(247, 76, 2);
    const colortype yellow = Color::getColorFrom24BitRGB(247, 210, 2);
}

inline float lerp(float a, float b, float t)
{
    float value =  a + t * (b - a);
    if( value < fminf(a,b))
    {
        return fminf(a,b);
    }
    if(value > fmaxf(a,b))
    {
        return fmaxf(a,b);
    }
    return value;
}

using namespace touchgfx;
inline colortype lerpColor(colortype x, colortype y, float alpha)
{
    float r = lerp((float)Color::getRed(x), (float)Color::getRed(y), alpha);
    float g = lerp((float)Color::getGreen(x), (float)Color::getGreen(y), alpha);
    float b = lerp((float)Color::getBlue(x), (float)Color::getBlue(y), alpha);
    return Color::getColorFrom24BitRGB(r,g,b);
}

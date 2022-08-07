/**
 * @file BoxRoundedCornersButtonStyle.hpp
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
#ifndef BOX_ROUNDED_CORNERS_BUTTON_STYLE_HPP
#define BOX_ROUNDED_CORNERS_BUTTON_STYLE_HPP

#include "gui/custom/BoxRoundedCorners.hpp"

namespace touchgfx
{
/**
 * @class BoxRoundedCornersButtonStyle BoxRoundedCornersButtonStyle.hpp touchgfx/containers/buttons/BoxRoundedCornersButtonStyle.hpp
 *
 * @brief A box with border button style.
 *
 *        A box with border button style. This class is supposed to be
 *        used with one of the ButtonTrigger classes to create a
 *        functional button. This class will show a box with a border
 *        in different colors depending on the state of the button
 *        (pressed or released).
 *
 *        An image button style. This class is supposed to be used
 *        with one of the ButtonTrigger classes to create a functional
 *        button. This class will show one of two images depending on
 *        the state of the button (pressed or released).
 *
 * @tparam T Generic type parameter. Typically a AbstractButtonContainer subclass.
 *
 * @see AbstractButtonContainer, Box
 */
template<class T>
class BoxRoundedCornersButtonStyle : public T
{
public:
    /**
     * @fn BoxRoundedCornersButtonStyle::BoxRoundedCornersButtonStyle()
     *
     * @brief Default constructor.
     */
    BoxRoundedCornersButtonStyle() : T(), up(), down()
    {
        T::add(box);
    }

    /**
     * @fn virtual BoxRoundedCornersButtonStyle::~BoxRoundedCornersButtonStyle()
     *
     * @brief Destructor.
     */
    virtual ~BoxRoundedCornersButtonStyle() { }

    /**
     * @brief Set the Box Corner Radius object
     * 
     * @param r 
     */
    void setBoxCornerRadius(int16_t r)
    {
        box.setCornerRadius(r);
    }

    /**
     * @brief Set the Box Position object
     * 
     * @param x 
     * @param y 
     * @param width 
     * @param height 
     */
    void setBoxPosition(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        box.setPosition(x, y, width, height);
    }

    /**
     * @brief Set the Box Width object
     * 
     * @param width 
     */
    void setBoxWidth(int16_t width)
    {
        box.setWidth(width);
    }

    /**
     * @brief Set the Box Height object
     * 
     * @param height 
     */
    void setBoxHeight(int16_t height)
    {
        box.setHeight(height);
    }

    /**
     * @fn void BoxRoundedCornersButtonStyle::setBoxColors(const colortype colorReleased, const colortype colorPressed, const colortype borderColorReleased, const colortype borderColorPressed)
     *
     * @brief Sets the colors.
     *
     * @param colorReleased The color released.
     * @param colorPressed  The color pressed.
     */
    void setBoxColors(const colortype colorReleased, const colortype colorPressed)
    {
        up = colorReleased;
        down = colorPressed;

        handlePressedUpdated();
    }

    // Hooks for replacing BoxRoundedCornersButtonStyle with BoxWithBorderButtonStyle
    // so auto-generated code can call this without method name modifications.
    // This allows you to do the following macro swap if you want all flex buttons in the designer to have rounded corners instead of borders:
    //   #define BoxWithBorderButtonStyle BoxRoundedCornersButtonStyle
    // Otherwise if these aren't called they should just optimize out
    void setBoxWithBorderPosition(int16_t x, int16_t y, int16_t width, int16_t height) { setBoxPosition(x, y, width, height); }
    void setBoxWithBorderWidth(int16_t width) { setBoxWidth(width); }
    void setBoxWithBorderHeight(int16_t height) { setBoxHeight(height); }
    void setBoxWithBorderColors(const colortype colorReleased, const colortype colorPressed, const colortype borderColorReleased, const colortype borderColorPressed) { setBoxColors(colorReleased, colorPressed); }
    void setBorderSize(uint8_t size) {}

protected:

    BoxRoundedCorners box;
    colortype up;
    colortype down;

    /**
     * @fn virtual void BoxRoundedCornersButtonStyle::handlePressedUpdated()
     *
     * @brief Handles the pressed updated.
     */
    virtual void handlePressedUpdated()
    {
        box.setColor(T::getPressed() ? down : up);
        T::handlePressedUpdated();
    }

    /**
     * @fn virtual void BoxRoundedCornersButtonStyle::handleAlphaUpdated()
     *
     * @brief Handles the alpha updated.
     */
    virtual void handleAlphaUpdated()
    {
        box.setAlpha(T::getAlpha());
        T::handleAlphaUpdated();
    }
};
} // namespace touchgfx

#endif // BOX_ROUNDED_CORNERS_BUTTON_STYLE_HPP

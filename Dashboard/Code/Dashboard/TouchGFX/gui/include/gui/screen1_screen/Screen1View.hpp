#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>


class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    void setText(touchgfx::TextAreaWithOneWildcard textArea, touchgfx::Unicode::UnicodeChar* buffer, const char inText[]);
    
    void setRPM(int inRPM);
    void setBatt(float inBatt);
    void setClt(int16_t inClt);
    void setGear(int inGear);
    void setOilPress(float inOilPress);
    void setOilTemp(uint8_t inOilTemp);

protected:
    int tickCounter = 0;
};

#endif // SCREEN1VIEW_HPP

#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <gui_generated/main_screen/MainViewBase.hpp>
#include <gui/main_screen/MainPresenter.hpp>

class MainView : public MainViewBase
{
public:
    MainView();
    virtual ~MainView() {}
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

#endif // MAINVIEW_HPP

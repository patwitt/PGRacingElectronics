#include <gui/screen1_screen/Screen1View.hpp>
#include <stdio.h>
#include <cstring>

Screen1View::Screen1View()
{
    rpmArea.setWildcard(rpmAreaBuffer);
    gearArea.setWildcard(gearAreaBuffer);
    cltArea.setWildcard(cltAreaBuffer);
    battArea.setWildcard(battAreaBuffer);

    setRPM(100);
    setGear(2);
    setClt(71.2);
    setBatt(13.42);
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
    Screen1View::tempBox1_1.redraw();
    tempBox1_1.invalidate();
    static int a = 0;
    if(tickCounter % 255 == 0)
    {
        a++; 
        a = a % 3;
        if(a == 0)
        {
            alertBar1.setState(EBarState::Info);
        }
        else
        {
            alertBar1.setState(EBarState::Warning);
        }
        
    }
    alertBar1.invalidate();
    alertBar1.setText("12345678912345\n");
    tickCounter++;
}

void Screen1View::setText(touchgfx::TextAreaWithOneWildcard textArea, touchgfx::Unicode::UnicodeChar* buffer, const char inText[])
{
    Unicode::snprintf(buffer, strlen(inText) + 1, inText);
    textArea.resizeToCurrentText();
    textArea.invalidate();
}

void Screen1View::setRPM(int inRPM)
{
    char textBuffer[10]; 
    sprintf(textBuffer,"%d", inRPM);
    setText(rpmArea, rpmAreaBuffer, textBuffer); 
}

void Screen1View::setBatt(float inBatt)
{
    char textBuffer[10];
    sprintf(textBuffer, "%.2f", inBatt);
    setText(battArea, battAreaBuffer, textBuffer);
}

void Screen1View::setClt(float inClt)
{
    char textBuffer[10];
    sprintf(textBuffer, "%.1f", inClt);
    setText(cltArea, cltAreaBuffer, textBuffer);
}

void Screen1View::setGear(int inGear)
{
    char textBuffer[10]; 
    sprintf(textBuffer,"%d", inGear);
    setText(gearArea, gearAreaBuffer, textBuffer); 
}

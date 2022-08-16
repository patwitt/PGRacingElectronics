#ifndef ALERTBAR_HPP
#define ALERTBAR_HPP

#include <gui_generated/containers/alertBarBase.hpp>
#include "ColorHelper.hpp"
#include <touchgfx/Color.hpp>

enum class EBarState : int
{
	Blank,
    Info,
    Warning,
    Fault,
	Critical1,
	Critical2,
    News
};

class alertBar : public alertBarBase
{
public:
    alertBar();
    virtual ~alertBar() {}

    virtual void initialize();
    void setState(EBarState inState);

    /* Color association with EBarState Enum */
    colortype dataColors[4] = {
        colors::blue,
        colors::orange,
        colors::red,
        colors::yellow
    };

    void setText(const char inText[]);

    touchgfx::Unicode::UnicodeChar frontTextBuffer[20];

protected:
};

#endif // ALERTBAR_HPP

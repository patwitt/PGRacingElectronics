#include <gui/containers/alertBar.hpp>
#include <string.h>

alertBar::alertBar()
{
}

void alertBar::initialize()
{
    alertBarBase::initialize();
    frontText.setWildcard(frontTextBuffer);
}

void alertBar::setState(EBarState inState)
{
    switch (inState)
    {
    case EBarState::Blank:
    	bgBox.setColor(colors::blue);
    	setText("");
    	break;
    case EBarState::Info:
        bgBox.setColor(colors::green);
        break;
    case EBarState::Warning:
        bgBox.setColor(colors::orange);
        break;
    case EBarState::Fault:
        bgBox.setColor(colors::red);
        break;
    case EBarState::Critical1:
    	bgBox.setColor(colors::red);
    	break;
    case EBarState::Critical2:
    	bgBox.setColor(colors::yellow);
    	break;
    case EBarState::News:
        bgBox.setColor(colors::blue);
        break;

    default:
        break;
    }
}

void alertBar::setText(const char inText[])
{
    Unicode::snprintf(frontTextBuffer, strlen(inText) + 1, inText);
    frontText.resizeToCurrentText();
    frontText.invalidate();
}

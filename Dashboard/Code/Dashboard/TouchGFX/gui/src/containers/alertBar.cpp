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
    case EBarState::Info:
        bgBox.setColor(dataColors[0]);
        break;
    case EBarState::Warning:
        bgBox.setColor(dataColors[1]);
        break;
    case EBarState::Fault:
        bgBox.setColor(dataColors[2]);
        break;
    case EBarState::News:
        bgBox.setColor(dataColors[3]);
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
#ifndef DEBUGINFOVIEW_HPP
#define DEBUGINFOVIEW_HPP

#include <gui_generated/debuginfo_screen/DebugInfoViewBase.hpp>
#include <gui/debuginfo_screen/DebugInfoPresenter.hpp>

class DebugInfoView : public DebugInfoViewBase
{
public:
    DebugInfoView();
    virtual ~DebugInfoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // DEBUGINFOVIEW_HPP

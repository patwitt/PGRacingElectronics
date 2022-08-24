#ifndef LANAVIEW_HPP
#define LANAVIEW_HPP

#include <gui_generated/lana_screen/LanaViewBase.hpp>
#include <gui/lana_screen/LanaPresenter.hpp>

class LanaView : public LanaViewBase
{
public:
    LanaView();
    virtual ~LanaView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // LANAVIEW_HPP

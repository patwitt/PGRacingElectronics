#ifndef DEBUGINFOPRESENTER_HPP
#define DEBUGINFOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DebugInfoView;

class DebugInfoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DebugInfoPresenter(DebugInfoView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~DebugInfoPresenter() {};

private:
    DebugInfoPresenter();

    DebugInfoView& view;
};

#endif // DEBUGINFOPRESENTER_HPP

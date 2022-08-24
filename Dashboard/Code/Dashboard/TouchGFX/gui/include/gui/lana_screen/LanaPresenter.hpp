#ifndef LANAPRESENTER_HPP
#define LANAPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class LanaView;

class LanaPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    LanaPresenter(LanaView& v);

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

    virtual ~LanaPresenter() {};

private:
    LanaPresenter();

    LanaView& view;
};

#endif // LANAPRESENTER_HPP

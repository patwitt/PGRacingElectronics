/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef DEBUGINFOVIEWBASE_HPP
#define DEBUGINFOVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/debuginfo_screen/DebugInfoPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/containers/Container.hpp>
#include <gui/containers/debugParameterBox.hpp>

class DebugInfoViewBase : public touchgfx::View<DebugInfoPresenter>
{
public:
    DebugInfoViewBase();
    virtual ~DebugInfoViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Container container1;
    debugParameterBox debugParameterBox1;
    debugParameterBox debugParameterBox2;
    debugParameterBox debugParameterBox3;
    debugParameterBox debugParameterBox4;
    debugParameterBox debugParameterBox5;
    debugParameterBox debugParameterBox6;
    touchgfx::Container container1_1;
    debugParameterBox debugParameterBox1_1;
    debugParameterBox debugParameterBox2_1;
    debugParameterBox debugParameterBox3_1;
    debugParameterBox debugParameterBox4_1;
    debugParameterBox debugParameterBox5_1;
    debugParameterBox debugParameterBox6_1;
    touchgfx::Container container1_2;
    debugParameterBox debugParameterBox1_2;
    debugParameterBox debugParameterBox2_2;
    debugParameterBox debugParameterBox3_2;
    debugParameterBox debugParameterBox4_2;
    debugParameterBox debugParameterBox5_2;
    debugParameterBox debugParameterBox6_2;
    touchgfx::Container container1_3;
    debugParameterBox debugParameterBox1_3;
    debugParameterBox debugParameterBox2_3;
    debugParameterBox debugParameterBox3_3;
    debugParameterBox debugParameterBox4_3;
    debugParameterBox debugParameterBox5_3;
    debugParameterBox debugParameterBox6_3;
    touchgfx::Container container1_4;
    debugParameterBox debugParameterBox1_4;
    debugParameterBox debugParameterBox2_4;
    debugParameterBox debugParameterBox3_4;
    debugParameterBox debugParameterBox4_4;
    debugParameterBox debugParameterBox5_4;
    debugParameterBox debugParameterBox6_4;
    touchgfx::Container container1_5;
    debugParameterBox debugParameterBox1_5;
    debugParameterBox debugParameterBox2_5;
    debugParameterBox debugParameterBox3_5;
    debugParameterBox debugParameterBox4_5;
    debugParameterBox debugParameterBox5_5;
    debugParameterBox debugParameterBox6_5;
    touchgfx::Container container1_6;
    debugParameterBox debugParameterBox1_6;
    debugParameterBox debugParameterBox2_6;
    debugParameterBox debugParameterBox3_6;
    debugParameterBox debugParameterBox4_6;
    debugParameterBox debugParameterBox5_6;
    debugParameterBox debugParameterBox6_6;
    touchgfx::Container container1_7;
    debugParameterBox debugParameterBox1_7;
    debugParameterBox debugParameterBox2_7;
    debugParameterBox debugParameterBox3_7;
    debugParameterBox debugParameterBox4_7;
    debugParameterBox debugParameterBox5_7;
    debugParameterBox debugParameterBox6_7;

private:

};

#endif // DEBUGINFOVIEWBASE_HPP
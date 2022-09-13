#ifndef ALERTBAR_HPP
#define ALERTBAR_HPP

#include <gui_generated/containers/alertBarBase.hpp>

class alertBar : public alertBarBase
{
public:
    alertBar();
    virtual ~alertBar() {}

    virtual void initialize();
protected:
};

#endif // ALERTBAR_HPP

#ifndef DEBUGPARAMETERBOX_HPP
#define DEBUGPARAMETERBOX_HPP

#include <gui_generated/containers/debugParameterBoxBase.hpp>

class debugParameterBox : public debugParameterBoxBase
{
public:
    debugParameterBox();
    virtual ~debugParameterBox() {}

    virtual void initialize();
protected:
};

#endif // DEBUGPARAMETERBOX_HPP

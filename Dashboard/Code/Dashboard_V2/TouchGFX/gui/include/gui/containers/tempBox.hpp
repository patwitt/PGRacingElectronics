#ifndef TEMPBOX_HPP
#define TEMPBOX_HPP

#include <gui_generated/containers/tempBoxBase.hpp>

class tempBox : public tempBoxBase
{
public:
    tempBox();
    virtual ~tempBox() {}

    virtual void initialize();
protected:
};

#endif // TEMPBOX_HPP

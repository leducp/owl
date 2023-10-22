#include "AbstractStepper.h"

namespace owl
{
    void AbstractStepper::setDirection(Direction dir)
    {
        direction_ = dir;
        if (dir == CW)
        {
            step_increment_ = 1;
        }
        else
        {
            step_increment_ = -1;
        }

        doDirection();
    }

    void AbstractStepper::step()
    {
        doStep();
        position_ += step_increment_;
    }
}

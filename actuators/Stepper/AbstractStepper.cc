#include "AbstractStepper.h"

namespace owl
{
    void AbstractStepper::tick()
    {
        if (direction_)
        {
            step_cw();
            ++position_;
        }
        else
        {
            step_ccw();
            --position_;
        }
    }
}

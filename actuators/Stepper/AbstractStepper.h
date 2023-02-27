#ifndef OWL_ABSTRACT_STEPPER_H
#define OWL_ABSTRACT_STEPPER_H

#include "../../common/Error.h"

namespace owl
{
    enum Direction
    {
        CW,
        CCW
    };

    class AbstractStepper
    {
    public:
        AbstractStepper() = default;
        virtual ~AbstractStepper() = default;

        virtual hresult setResolution(uint16_t resolution) { return E_NOT_SUPPORTED; }
        int resolution() const { return resolution_; }

        void setDirection(enum Direction dir);
        bool direction() const { return direction_; }

        void step();

        virtual hresult sleep(bool shallSleep) { return E_NOT_SUPPORTED; }

    protected:
        virtual void doStep();

        int resolution_{1};
        int step_increment_{1};
        
        Direction direction_{Direction::CW};
        int32_t position_{0};
    };
}

#endif

#ifndef OWL_ABSTRACT_STEPPER_H
#define OWL_ABSTRACT_STEPPER_H

#include "../../common/Error.h"

namespace owl
{
    class AbstractStepper
    {
    public:
        AbstractStepper() = default;
        virtual ~AbstractStepper() = default;

        virtual hresult setStepResolution(uint16_t resolution) { return E_NOT_SUPPORTED; }
        int resolution() const { return resolution_; }

        void setDirection(bool dir) { direction_ = dir; }
        bool direction() const { return direction_; }

        void tick();

        virtual hresult sleep(bool shallSleep) { return E_NOT_SUPPORTED; }

    protected:
        virtual void step_cw() {};
        virtual void step_ccw() {};

        int resolution_{1};
        
        bool direction_{true};
        int32_t position_{0};
    };
}

#endif

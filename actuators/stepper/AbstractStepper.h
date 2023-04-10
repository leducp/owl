#ifndef OWL_ABSTRACT_STEPPER_H
#define OWL_ABSTRACT_STEPPER_H

#include "Error.h"

namespace owl
{
    class AbstractStepper
    {
    public:
        enum Direction
        {
            CW,
            CCW
        };

        AbstractStepper() = default;
        virtual ~AbstractStepper() = default;

        virtual hresult setResolution(uint16_t resolution) { return error::E_NOT_SUPPORTED; }
        int resolution() const { return resolution_; }

        void setDirection(Direction dir);
        bool direction() const { return direction_; }

        void step();

        virtual hresult standby(bool enableStandby) { return error::E_NOT_SUPPORTED; }

    protected:
        virtual void doStep() = 0;
        virtual void doDirection() = 0;

        int resolution_{1};
        int step_increment_{1};

        Direction direction_{Direction::CW};
        int32_t position_{0};
    };
}

#endif

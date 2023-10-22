#ifndef OWL_A4988_H
#define OWL_A4988_H

#include "AbstractStepper.h"
#include "hal/gpio.h"

namespace owl
{
    class A4988 final : public AbstractStepper
    {
    public:
        A4988(gpio::Pad step, gpio::Pad dir, gpio::Pad sleep,
              gpio::Pad m0, gpio::Pad m1, gpio::Pad m2);
        ~A4988() override = default;

        hresult standby(bool enableStandby) override;
        hresult setResolution(uint16_t resolution) override;

    private:
        void doStep() override;
        void doDirection() override;

        gpio::Pad step_;
        gpio::Pad dir_;
        gpio::Pad sleep_;

        gpio::Pad m0_;
        gpio::Pad m1_;
        gpio::Pad m2_;
    };
}

#endif

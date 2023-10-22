#ifndef OWL_ULN200X_H
#define OWL_ULN200X_H

#include "AbstractStepper.h"
#include "hal/gpio.h"

namespace owl
{
    constexpr uint8_t CW_SEQUENCES[][8]
    {
        {0x03,0x06,0x0c,0x09},
        {0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09}
    };

    constexpr uint8_t CCW_SEQUENCES[][8]
    {
        {0x09,0x0c,0x06,0x03},
        {0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08}
    };

    class ULN200x final : public AbstractStepper
    {
    public:
        ULN200x(gpio::Port port, int offset);
        ~ULN200x() override = default;

        hresult standby(bool enableStandby) override;
        hresult setResolution(uint16_t resolution) override;

    private:
        void doStep() override;
        void doDirection() override;

        uint8_t const* sequence_;
        uint8_t mask_;
        uint8_t index_{0};

        gpio::Port port_;
        int offset_;
    };
}

#endif

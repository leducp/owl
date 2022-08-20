#ifndef OWL_ULN200X_H
#define OWL_ULN200X_H

#include "AbstractStepper.h"

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

    class ULN200x : public AbstractStepper
    {
    public:
        ULN200x();
        virtual ~ULN200x();

        hresult sleep(bool shallSleep) override;
        hresult setStepResolution(uint16_t resolution) override;

    private:
        void step_cw() override;
        void step_ccw() override;

        uint8_t const* cw_;
        uint8_t const* ccw_;
        uint8_t mask_;
        uint8_t index_{0};
    };
}

#endif

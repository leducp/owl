#include "ULN200x.h"
#include <avr/io.h>

namespace owl
{
    ULN200x::ULN200x()
    {
        cw_  = CW_SEQUENCES[0];
        ccw_ = CCW_SEQUENCES[0];
        mask_ = 3;
        index_ = 0;
    }

    hresult ULN200x::sleep(bool shallSleep)
    {
        return E_NOT_SUPPORTED;
    }

    hresult ULN200x::setResolution(uint16_t resolution)
    {
        if (resolution > 2)
        {
            return E_NOT_SUPPORTED;
        }

        resolution_ = resolution;
        cw_  = CW_SEQUENCES[resolution-1];
        ccw_ = CCW_SEQUENCES[resolution-1];
        mask_ = (4 * resolution_) - 1;

        return S_OK;
    }


    void ULN200x::doStep()
    {
        if (direction_ == CW)
        {
            PORTD = cw_[index_ & mask_] << 3;
        }
        else
        {
            PORTD = ccw_[index_ & mask_] << 3;
        }
        ++index_;
    }
}

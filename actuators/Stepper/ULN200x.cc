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

    ULN200x::~ULN200x()
    {
        
    }

    hresult ULN200x::sleep(bool shallSleep)
    {
        return E_NOT_SUPPORTED;
    }

    hresult ULN200x::setStepResolution(uint16_t resolution)
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

    void ULN200x::step_cw() 
    {
        PORTD = cw_[index_ & mask_] << 3;
        ++index_;
    }

    void ULN200x::step_ccw() 
    {
        PORTD = ccw_[index_ & mask_] << 3;
        ++index_;
    }

}

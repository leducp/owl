#include "ULN200x.h"

namespace owl
{
    ULN200x::ULN200x(gpio::Port port, int offset)
        : port_{port}
        , offset_{offset}
    {
        sequence_  = CW_SEQUENCES[0];
        mask_ = 3;
        index_ = 0;
    }

    hresult ULN200x::standby(bool enableStandby)
    {
        return E_NOT_SUPPORTED;
    }

    hresult ULN200x::setResolution(uint16_t resolution)
    {
        if ((resolution > 2) or (resolution < 1))
        {
            return E_NOT_SUPPORTED;
        }

        resolution_ = resolution;
        mask_ = (4 * resolution_) - 1;
        doDirection(); // Update sequence in use

        return S_OK;
    }

    void ULN200x::doStep()
    {
        port_ = sequence_[index_ & mask_] << offset_;
        ++index_;
    }

    void ULN200x::doDirection()
    {
        if (direction_ == CW)
        {
            sequence_ = CW_SEQUENCES[resolution_ - 1];
        }
        else
        {
            sequence_ = CCW_SEQUENCES[resolution_ - 1];
        }
    }
}

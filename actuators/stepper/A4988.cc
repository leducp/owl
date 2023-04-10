#include "A4988.h"
#include "Time.h"

namespace owl
{
    A4988::A4988(gpio::Pad step, gpio::Pad dir, gpio::Pad sleep,
                 gpio::Pad m0, gpio::Pad m1, gpio::Pad m2)
        : step_{step}
        , dir_{dir}
        , sleep_{sleep}
        , m0_{m0}
        , m1_{m1}
        , m2_{m2}
    {
        step_.setDirection(gpio::Direction::OUT);
        dir_.setDirection(gpio::Direction::OUT);
        sleep_.setDirection(gpio::Direction::OUT);
        m0_.setDirection(gpio::Direction::OUT);
        m1_.setDirection(gpio::Direction::OUT);
        m2_.setDirection(gpio::Direction::OUT);

        setResolution(1);
        standby(false);
    }

    hresult A4988::standby(bool enableStandby)
    {
        sleep_ = enableStandby;
        return error::S_OK;
    }

    hresult A4988::setResolution(uint16_t resolution)
    {
        switch (resolution)
        {
            case 1:  { m0_ = false; m1_ = false; m2_ = false; return error::S_OK; }
            case 2:  { m0_ = true;  m1_ = false; m2_ = false; return error::S_OK; }
            case 4:  { m0_ = false; m1_ = true;  m2_ = false; return error::S_OK; }
            case 8:  { m0_ = true;  m1_ = true;  m2_ = false; return error::S_OK; }
            case 16: { m0_ = true;  m1_ = true;  m2_ = true;  return error::S_OK; }
            default:
            {
                return error::E_INVALID;
            }
        }
    }

    void A4988::doStep()
    {
        step_ = true;
        sleep(2us);
        step_ = false;
        sleep(2us);
    }

    void A4988::doDirection()
    {
        if (direction_ == CW)
        {
            dir_ = true;
        }
        else
        {
            dir_ = false;
        }
    }
}

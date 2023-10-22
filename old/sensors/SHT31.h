#ifndef OWL_SHT31_H
#define OWL_SHT31_H

#include "hal/i2c.h"

namespace owl
{
    class SHT31
    {
    public:
        SHT31(I2C& bus);
        ~SHT31() = default;

        hresult fetch();
        int temperature() const { return temperature_;  }
        int humidity() const    { return humidity_;     }
        int precision() const   { return precision_;    }

    private:
        static constexpr uint8_t const address_{0x44};
        static constexpr uint8_t const precision_ = 10;

        I2C& bus_;
        int temperature_;
        int humidity_;
    };
}

#endif

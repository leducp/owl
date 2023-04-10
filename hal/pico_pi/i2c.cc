#include <cstdlib>
#include <cstdio>

#include "hal/i2c.h"
#include "hardware/i2c.h"

namespace owl
{
    I2C::I2C(int instance, int frequency)
    {
        switch (instance)
        {
            case 0:  { bus_ = i2c0;  break; }
            case 1:  { bus_ = i2c1;  break; }
            default: { std::abort(); }
        }

        i2c_init(static_cast<i2c_inst_t*>(bus_), frequency);
    }

    I2C::~I2C()
    {
        if (bus_ != nullptr)
        {
            i2c_deinit(static_cast<i2c_inst_t*>(bus_));
        }
    }

    hresult I2C::transmit(uint8_t address,
            uint8_t const* txbytes, uint8_t tx_size,
            uint8_t* rxbytes, uint8_t rx_size, nanoseconds timeout)
    {
        (void) timeout; // Not supported yet

        if (txbytes != nullptr)
        {
            bool repeated_start = (rxbytes != nullptr);
            int r = i2c_write_blocking(static_cast<i2c_inst_t*>(bus_), address, txbytes, tx_size, repeated_start);
            if (r < 0)
            {
                return E_GENERIC;
            }
        }

        if (rxbytes != nullptr)
        {
            int r = i2c_read_blocking(static_cast<i2c_inst_t*>(bus_), address, rxbytes, rx_size, false);
            if (r < 0)
            {
                return E_GENERIC;
            }
        }

        return error::S_OK;
    }
}

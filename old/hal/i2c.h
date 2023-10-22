#ifndef OWL_I2C_H
#define OWL_I2C_H

#include "Error.h"
#include "Time.h"

namespace owl
{
    class I2C
    {
    public:
        I2C(int instance, int frequency);
        ~I2C();

        // Transmit bytes. Always write then read bytes with a repeated start between.
        // If txbytes == nullptr, only read
        // If rxbytes == nullptr, only write
        // timeout < 0 -> no timeout
        hresult transmit(uint8_t address, 
            uint8_t const* txbytes, uint8_t tx_size,
            uint8_t* rxbytes, uint8_t rx_size, nanoseconds timeout = -1ns);

        hresult read (uint8_t address, uint8_t* rxbytes, uint8_t rx_size, nanoseconds timeout = -1ns)
        {
            return transmit(address, nullptr, 0, rxbytes, rx_size, timeout);
        }

        hresult write(uint8_t address, uint8_t const* txbytes, uint8_t tx_size, nanoseconds timeout = -1ns)
        {
            return transmit(address, txbytes, tx_size, nullptr, 0, timeout);
        }

    private:
        void* bus_{nullptr};
    };
}

#endif

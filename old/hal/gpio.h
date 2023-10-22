#ifndef OWL_GPIO_H
#define OWL_GPIO_H

#include "hardware_types.h"

namespace owl::gpio
{
    enum Direction
    {
        IN,
        OUT
    };

    // A GPIO port
    class Port
    {
    public:
        Port(int port, port_size mask);
        ~Port();

        Port& operator=(port_size bits);
        operator port_size();

    private:
        port_size mask_;
    };

    // A single GPIO pad
    class Pad
    {
    public:
        // If the uC have multiple port, then pin is defined as port_size * port_pin + pin_number.
        // Example: 32bit uC, pin 3 of port D: pin = 32 * 4 + 3
        Pad(int pin);
        ~Pad();

        void setDirection(Direction dir);

        Pad& operator=(bool value);
        operator bool();

    private:
        int pin_;
    };
}

#endif

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

        void put(bool value);
        void set();
        void clear();
        void toogle();
        port_size get();

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
        void put(bool value);
        void set();
        void clear();
        void toogle();
        bool get();
    };
}

#endif

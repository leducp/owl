#include "hal/gpio.h"
#include "pico/stdlib.h"

namespace owl::gpio
{
    // Only one port on pico pi
    Port::Port(int, port_size mask)
        : mask_{mask}
    { }

    Port::~Port()
    { }

    Port& Port::operator=(port_size bits)
    {
        gpio_put_masked(mask_, bits);
        return *this;
    }

    Port::operator port_size()
    {
        return gpio_get_all() & mask_;
    }


    Pad::Pad(int pin)
        : pin_{pin} // only one port on pico pi -> no need for translation
    { 
        gpio_init(pin_);
    }

    Pad::~Pad()
    { }
    
    void Pad::setDirection(Direction dir)
    {
        if (dir == OUT)
        {
            gpio_set_dir(pin_, true);
        }
        else
        {
            gpio_set_dir(pin_, false);
        }
    }

    Pad& Pad::operator=(bool value)
    {
        gpio_put(pin_, value);
        return *this;
    }

    Pad::operator bool()
    {
        return gpio_get(pin_);
    }
}

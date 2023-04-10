#include "actuators/stepper/A4988.h"
#include "Time.h"


#include "pico/stdlib.h"
#include "hardware/pwm.h"

using namespace owl;

constexpr nanoseconds SPEED = 200us;

int main()
{
    gpio::Pad dir(13);
    gpio::Pad step(14);
    gpio::Pad sleep(15);

    gpio::Pad m0(18);
    gpio::Pad m1(19);
    gpio::Pad m2(20);

    gpio::Pad reset(21);
    reset.setDirection(gpio::Direction::OUT);
    reset = 1;

    A4988 driver(step, dir, sleep, m0, m1, m2);
    driver.setResolution(16);

    while (true)
    {
        for (int i = 0; i<10000; ++i)
        {
            driver.step();
            owl::sleep(SPEED);
        }
        owl::sleep(100ms);
        driver.setDirection(AbstractStepper::CW);
        
        for (int i = 0; i<10000; ++i)
        {
            driver.step();
            owl::sleep(SPEED);
        }
        owl::sleep(100ms);
        driver.setDirection(AbstractStepper::CCW);
    }
    return 0;
}

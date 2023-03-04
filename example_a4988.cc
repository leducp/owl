#include "actuators/stepper/A4988.h"
#include "Time.h"

using namespace owl;

constexpr nanoseconds SPEED = 2ms;

int main()
{
    gpio::Pad step(16);
    gpio::Pad dir(17);
    gpio::Pad sleep(18);

    gpio::Pad m0(19);
    gpio::Pad m1(20);
    gpio::Pad m2(21);

    A4988 driver(step, dir, sleep, m0, m1, m2);

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

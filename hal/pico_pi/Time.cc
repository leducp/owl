#include "Time.h"
#include "pico/stdlib.h"

namespace owl
{
    void sleep(nanoseconds duration)
    {
        sleep_us(duration_cast<microseconds>(duration).count());
    }
}

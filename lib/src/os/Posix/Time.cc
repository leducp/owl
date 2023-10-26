#include "Time.h"
#include <thread>

namespace owl
{
    void sleep(nanoseconds ns)
    {
        std::this_thread::sleep_for(ns);
    }
}

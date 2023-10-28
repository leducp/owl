#ifndef OWL_LIB_TIME_H
#define OWL_LIB_TIME_H

#include <chrono>

namespace owl
{
    using namespace std::chrono;
    using seconds_f = std::chrono::duration<float>;

    void sleep(nanoseconds duration);

    nanoseconds since_epoch();
    nanoseconds elapsed_time(nanoseconds start);
}

#endif

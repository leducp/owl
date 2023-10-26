#ifndef OWL_LIB_TIME_H
#define OWL_LIB_TIME_H

#include <chrono>

namespace owl
{
    using namespace std::chrono;

    void sleep(nanoseconds duration);
}

#endif

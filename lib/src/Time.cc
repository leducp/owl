#include "Time.h"

namespace owl
{
    nanoseconds since_epoch()
    {
        auto now = time_point_cast<nanoseconds>(system_clock::now());
        return now.time_since_epoch();
    }

    nanoseconds elapsed_time(nanoseconds start)
    {
        return since_epoch() - start;
    }
}

#include "os/Mutex.h"

namespace owl
{
    LockGuard::LockGuard(Mutex& mutex)
        : mutex_{mutex}
    {
        (void) mutex_.lock();
    }

    LockGuard::~LockGuard()
    {
        (void) mutex_.unlock();
    }
}

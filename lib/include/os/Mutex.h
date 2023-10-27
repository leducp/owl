#ifndef OWL_LIB_OS_MUTEX_H
#define OWL_LIB_OS_MUTEX_H

#include "os_types.h"

namespace owl
{
    class Mutex
    {
    public:
        Mutex();
        ~Mutex();

        int lock();
        int unlock();

    private:
        os_mutex mutex_;
    };

    class LockGuard
    {
    public:
        LockGuard(Mutex& mutex);
        ~LockGuard();

    private:
        Mutex& mutex_;
    };
}

#endif

#include "os/Mutex.h"

namespace owl
{
    Mutex::Mutex()
        : mutex_{}
    {
        mutex_ = PTHREAD_MUTEX_INITIALIZER;
    }

    Mutex::~Mutex()
    {

    }

    int Mutex::lock()
    {
        return pthread_mutex_lock(&mutex_);
    }

    int Mutex::unlock()
    {
        return pthread_mutex_unlock(&mutex_);
    }
}

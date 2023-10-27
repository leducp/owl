#ifndef OWL_LIB_OS_POSIX_OS_TYPES_H
#define OWL_LIB_OS_POSIX_OS_TYPES_H

#include <pthread.h>

namespace owl
{
    using os_device = int;
    using os_mutex = pthread_mutex_t;
}

#endif

#ifndef OWL_HARDWARE_TYPES_H
#define OWL_HARDWARE_TYPES_H

#include "Error.h"

namespace owl
{
    using port_size = uint32_t;
    using os_device = int;

    hresult errnoToHresult(int error);
}

#endif

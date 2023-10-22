#include <errno.h>
#include <cstring>
#include "hardware_types.h"

namespace owl
{
    hresult errnoToHresult(int error)
    {
        switch (error)
        {
            case EINVAL:    { return error::E_INVALID;          }
            case ENOTSUP:   { return error::E_NOT_SUPPORTED;    }
            default:
            {
                return hresult(error | 0x81000000, std::strerror(errno));
            }
        }
    }
}

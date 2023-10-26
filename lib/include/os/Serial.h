#ifndef OWL_LIB_OS_SERIAL_H
#define OWL_LIB_OS_SERIAL_H

#include <string>
#include "os_types.h"

namespace owl
{
    class Serial
    {
    public:
        Serial();
        ~Serial();

        int open(std::string const& device, int baudrate);
        void close();

        int32_t read (void* buffer, int32_t size);
        int32_t write(void const* buffer, int32_t size);

    private:
        os_device line_;
    };
}

#endif

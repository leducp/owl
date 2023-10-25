#include <unistd.h>

#include "FramePosix.h"

namespace owl
{
    FramePosix::FramePosix(int fd)
        : fd_{fd}
    { }

    int FramePosix::write_byte(uint8_t c)
    {
        return ::write(fd_, &c, 1);
    }

    int FramePosix::read_byte(uint8_t& c)
    {
        return ::read(fd_, &c, 1);
    }
}

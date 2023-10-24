#include <unistd.h>

#include "FramePosix.h"

namespace owl
{
    FramePosix::FramePosix(int fd)
        : fd_{fd}
    { }

    int FramePosix::write_byte(char c)
    {
        return ::write(fd_, &c, 1);
    }

    int FramePosix::read_byte(char& c)
    {
        return ::read(fd_, &c, 1);
    }
}

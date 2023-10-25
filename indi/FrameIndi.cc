#include <indicom.h>
#include <indilogger.h>

#include "FrameIndi.h"

const char *getDeviceName()
{
    return "owl::FrameIndi";
}

namespace owl
{
    FrameIndi::FrameIndi(int fd)
        : fd_{fd}
    { }

    void FrameIndi::setPort(int fd)
    {
        fd_ = fd;
    }

    int FrameIndi::write_byte(uint8_t c)
    {
        int w;
        tty_write(fd_, (char*)&c, 1, &w);
        if (w == 0)
        {
            return -1;
        }
        return 1;
    }

    int FrameIndi::read_byte(uint8_t& c)
    {
        int r;
        tty_read(fd_, (char*)&c, 1, 0, &r);
        if (r == 0)
        {
            return -1;
        }
        //LOGF_INFO("read %x\n", c);
        return 1;
    }
}

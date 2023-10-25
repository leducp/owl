#ifndef OWL_LIB_PROTOCOL_FRAME_POSIX_H
#define OWL_LIB_PROTOCOL_FRAME_POSIX_H

#include "Frame.h"

namespace owl
{
    class FramePosix final : public Frame
    {
    public:
        FramePosix(int fd);

    private:
        int write_byte(uint8_t c) override;
        int read_byte(uint8_t& c) override;

        int fd_;
    };
}

#endif

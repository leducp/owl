#ifndef OWL_LIB_PROTOCOL_FRAME_POSIX_H
#define OWL_LIB_PROTOCOL_FRAME_POSIX_H

#include "Frame.h"

namespace owl
{
    class FramePosix final : public Frame
    {
    public:
        FramePosix(int fd);

    protected:
        int write_byte(char c) override;
        int read_byte(char& c) override;

    private:
        int fd_;
    };
}

#endif

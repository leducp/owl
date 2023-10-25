#ifndef OWL_INDI_FRAME_INDI_H
#define OWL_INDI_FRAME_INDI_H

#include "Frame.h"

namespace owl
{
    class FrameIndi final : public Frame
    {
    public:
        FrameIndi(int fd);
        void setPort(int fd);

    private:
        int write_byte(uint8_t c) override;
        int read_byte(uint8_t & c) override;

        int fd_;
    };
}

#endif

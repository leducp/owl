#ifndef OWL_LIB_PROTOCOL_FRAME_QUEUE_H
#define OWL_LIB_PROTOCOL_FRAME_QUEUE_H

#include <queue>

#include "Frame.h"

namespace owl
{
    // To simulate a com
    class FrameQueue final : public Frame
    {
    public:
        FrameQueue(std::queue<uint8_t>* channel);

    private:
        int write_byte(uint8_t c) override;
        int read_byte(uint8_t& c) override;

        std::queue<uint8_t>* channel_;
    };
}

#endif

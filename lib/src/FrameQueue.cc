#include "FrameQueue.h"

namespace owl
{
    FrameQueue::FrameQueue(std::queue<uint8_t>* channel)
        : channel_(channel)
    {

    }

    int FrameQueue::write_byte(uint8_t c)
    {
        channel_->push(c);
        return 1;
    }

    int FrameQueue::read_byte(uint8_t& c)
    {
        if (channel_->empty())
        {
            return -1;
        }
        c = channel_->front();
        channel_->pop();
        return 1;
    }
}

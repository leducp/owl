#ifndef OWL_LIB_FRAME_H
#define OWL_LIB_FRAME_H

#include <cstring>
#include <cstdint>
#include <queue>
#include "Hash.h"

namespace owl
{
    class Frame
    {
        static constexpr int MAX_SIZE = 128;
        static constexpr char DELIMITER = 0x7e;
        static constexpr char ESCAPE = 0x7d;
    public:
        Frame();
        virtual ~Frame() = default;

        void reset();

        template<typename T>
        bool fetch(T& data)
        {
            if (not is_ready_)
            {
                return false;
            }

            std::memcpy(&data, data_.data(), std::min(sizeof(T), data_.size()));
            reset();

            return true;
        }

        bool read();
        bool write(void const* data, int size);

        char const* data() const { return data_.data(); }
        std::size_t size() const { return data_.size(); }

    protected:
        virtual int write_byte(char c) = 0;
        virtual int read_byte(char& c) = 0;

    private:
        std::unique_ptr<Hash> hash_{};
        std::vector<char> data_{};
        bool is_started_{false};
        bool is_ready_{false};
        bool is_escaped_{false};
    };

    class FrameBuffer final : public Frame
    {
    public:
        FrameBuffer(std::queue<char>* channel)
            : channel_(channel)
        {

        }

    protected:
        int write_byte(char c) override
        {
            channel_->push(c);
            return 1;
        }

        int read_byte(char& c) override
        {
            if (channel_->empty())
            {
                return -1;
            }
            c = channel_->front();
            channel_->pop();
            return 1;
        }

    private:
        std::queue<char>* channel_;
    };
}

#endif

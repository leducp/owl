#ifndef OWL_LIB_FRAME_H
#define OWL_LIB_FRAME_H

#include <cstring>
#include <cstdint>
#include "Hash.h"

namespace owl
{
    class Frame
    {
        static const int MAX_SIZE = 128;
        static const char DELIMITER = 0x7e;
        static const char ESCAPE = 0x7d;
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

        /// Read data. return true if a frame is ready to work on.
        bool read();

        // Write a whole frame in a row
        bool write(void const* data, int size);

        bool write_start();
        void write_data(void const* data, int size);
        void write_finalize();

        uint8_t const* data() const { return data_.data(); }
        std::size_t size() const { return data_.size(); }

    protected:
        virtual int write_byte(uint8_t c) = 0;
        virtual int read_byte(uint8_t& c) = 0;

    private:
        std::unique_ptr<Hash> hash_{};
        std::vector<uint8_t> data_{};
        bool is_started_{false};
        bool is_ready_{false};
        bool is_escaped_{false};
    };
}

#endif

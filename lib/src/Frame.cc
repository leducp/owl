#include "Frame.h"

namespace owl
{
    Frame::Frame()
    {
        hash_ = createHash("crc16/x25");
        hash_->init();
        data_.reserve(MAX_SIZE);
    }

    void Frame::reset()
    {
        is_ready_   = false;
        is_started_ = false;
        is_escaped_ = false;
        data_.clear();
    }

    bool Frame::read()
    {
        if (is_ready_)
        {
            return true;
        }

        while (not is_started_)
        {
            uint8_t c;
            if (read_byte(c) < 0)
            {
                return false;
            }
            if (c == DELIMITER)
            {
                is_started_ = true;
            }
        }

        while (true)
        {
            uint8_t c;
            if (read_byte(c) < 0)
            {
                return false;
            }

            if (is_escaped_)
            {
                is_escaped_ = false;
                c ^= (1 << 5);
                data_.push_back(c);
                continue;
            }

            if (c == DELIMITER)
            {
                // end of frame ?
                if (data_.size() < 2)
                {
                    // start of another frame
                    continue;
                }

                // Extract received CRC
                uint8_t crc0 = data_.at(data_.size() - 1);
                uint8_t crc1 = data_.at(data_.size() - 2);
                uint16_t crc = (crc0 << 8) | crc1;
                data_.resize(data_.size() - 2);

                // compute CRC on received bytes
                hash_->update(data_);
                uint16_t computedCRC = hash_->finalize<uint16_t>();

                // compare CRC
                if (crc != computedCRC)
                {
                    //printf("wrong CRC (%02x vs %02x) - %d\n",
                    //    crc, computedCRC, data_.size());
                    reset();
                    return false;
                }

                is_ready_ = true;
                return true;
            }

            if (c == ESCAPE)
            {
                is_escaped_ = true;
                continue;
            }

            data_.push_back(c);
            continue;
        }
    }

    bool Frame::write(void const* data, int size)
    {
        if (not write_start())
        {
            return false;
        }

        write_data(data, size);

        write_finalize();
        return true;
    }

    bool Frame::write_start()
    {
        if (write_byte(DELIMITER) < 0)
        {
            return false;
        }
        return true;
    }

    void Frame::write_data(void const* data, int size)
    {
        uint8_t const* pos = reinterpret_cast<uint8_t const*>(data);
        for (int i = 0; i < size; ++i)
        {
            uint8_t to_send = pos[i];

            hash_->update(to_send);
            if ((to_send == DELIMITER) or (to_send == ESCAPE))
            {
                write_byte(ESCAPE);
                to_send ^= (1 << 5);
            }
            write_byte(to_send);
        }
    }

    void Frame::write_finalize()
    {
        uint16_t crc = hash_->finalize<uint16_t>();
        write_byte((crc & 0x00ff) >> 0);
        write_byte((crc & 0xff00) >> 8);

        write_byte(DELIMITER);
    }
}

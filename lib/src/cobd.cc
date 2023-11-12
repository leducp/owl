#include "cobd.h"
#include <cstdio>

namespace owl
{
    std::vector<uint8_t> encode(std::vector<uint8_t> const& buffer)
    {
        std::vector<uint8_t> out;
        out.reserve(buffer.size() + 2); // at least current size + start + end - TODO: reserve depending of number of 0?
        out.push_back(0);
        std::size_t pos = 0;       // position of the current counter
        uint8_t counter = 1;    // current counter value

        for (auto const& byte : buffer)
        {
            out.push_back(byte);
            if (byte == DELIMITER)
            {
                out[pos] = counter;
                pos = out.size() - 1;
                counter = 1;
                continue;
            }

            ++counter;
            if (counter == 0xff)
            {
                out[pos] = counter;
                out.push_back(0);
                pos = out.size() - 1;
                counter = 1;
            }
        }

        // Finalize frame
        out[pos] = counter;
        out.push_back(DELIMITER);

        return out;
    }

    std::vector<uint8_t> decode(std::vector<uint8_t> const& buffer)
    {
        std::vector<uint8_t> out;
        std::size_t pos = 0;
        while (pos < buffer.size())
        {
            uint8_t const& counter = buffer[pos];
            ++pos;

            for (uint8_t i = 0; i < (counter - 1); ++i)
            {
                uint8_t data = buffer[pos];
                ++pos;
                if (data == 0)
                {
                    // bug? shouldn't happen right? (sicne we are in the middle of a block...)
                    throw "aie";
                }

                out.push_back(data);
            }

            if (buffer[pos] == 0)
            {
                // end of packet!
                // check pos for coherency?
                return out;
            }

            if (counter != 0xff)
            {
                // if counter == 0xff, it means there is no 0
                out.push_back(0);
            }
        }

        return out;
    }
}

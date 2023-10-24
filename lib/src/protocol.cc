#include <cstring>
#include <unistd.h>

#include "protocol.h"

namespace owl
{
    void write_trace(int fd, char const* message)
    {
        owl::Header header;
        header.type = owl::Type::TRACE;
        /*
        header.size = strlen(message);
        if (header.size > 128)
        {
            header.size = 128;
        }
        */
        header.size = 4U;
        ::write(fd, &header, sizeof(owl::Header));
        ::write(fd, message, 4);
    }

    void write_feedback(int fd, Feedback const& feedback)
    {
        owl::Header header;
        header.type = owl::Type::FEEDBACK;
        header.size = sizeof(owl::Feedback);

        ::write(fd, &header, sizeof(owl::Header));
        ::write(fd, &feedback, header.size);
    }

    int readData(int fd, uint8_t* buffer, int size)
    {
        int count = 0;
        while (count != size)
        {
            int r = ::read(fd, buffer + count, size - count);
            if (r < 0)
            {
                return r;
            }
            count += r;
        }

        return count;
    }
}

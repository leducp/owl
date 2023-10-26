#include <cstring>
#include <unistd.h>

#include "protocol.h"

namespace owl
{
    void write_trace(Frame& frame, char const* message)
    {
        frame.write_start();

        Header header;
        header.type = Type::TRACE;
        header.size = strlen(message);
        if (header.size > 128)
        {
            header.size = 128;
        }

        frame.write_data(&header, sizeof(Header));
        frame.write_data(message, header.size);

        frame.write_finalize();
    }

    void write_feedback(Frame& frame, Feedback const& feedback)
    {
        frame.write_start();

        Header header;
        header.type = Type::FEEDBACK;
        header.size = sizeof(Feedback);

        frame.write_data(&header, sizeof(Header));
        frame.write_data(&feedback, header.size);

        frame.write_finalize();
    }

    void write_control(Frame& frame, Control const& control)
    {
        frame.write_start();

        Header header;
        header.type = Type::CONTROL;
        header.size = sizeof(Control);

        frame.write_data(&header, sizeof(Header));
        frame.write_data(&control, header.size);

        frame.write_finalize();
    }

    void write_refresh(Frame& frame)
    {
        frame.write_start();

        Header header;
        header.type = Type::REFRESH;
        header.size = 0;

        frame.write_data(&header, sizeof(Header));

        frame.write_finalize();
    }

    void write_gps(Frame& frame, GPS const& gps)
    {
        frame.write_start();

        Header header;
        header.type = Type::GPS;
        header.size = sizeof(GPS);

        frame.write_data(&header, sizeof(Header));
        frame.write_data(&gps, header.size);

        frame.write_finalize();
    }
}

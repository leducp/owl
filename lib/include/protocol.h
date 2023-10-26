#ifndef OWL_LIB_PROTOCOL
#define OWL_LIB_PROTOCOL

#include "Frame.h"

namespace owl
{
    enum class Type : uint8_t
    {
        TRACE = 1,
        FEEDBACK,
        CONTROL,
        REFRESH,
        GPS
    };

    enum class State : int32_t
    {
        READY,
        BUSY,
        FAULT
    };

    struct Header
    {
        enum Type type;
        uint8_t size;
    } __attribute__((__packed__));

    struct Feedback
    {
        enum State sensor_state;
        float temperature;
        float humidity;

        enum State focus_state;
        int32_t focus_pos;
    } __attribute__((__packed__));

    struct GPS
    {
        enum State state;
        float latitude;
        float longitude;
        float altitude;
        int time;
        int date;
        int fix_quality;
        int satellites;
        float dop;
    } __attribute__((__packed__));

    struct Control
    {
        int32_t steps;
        int32_t speed;
        uint8_t outputs[7];
    } __attribute__((__packed__));

    void write_trace(Frame& frame, char const* message);
    void write_feedback(Frame& frame, Feedback const& feeback);
    void write_control(Frame& frame, Control const& control);
    void write_gps(Frame& frame, GPS const& gps);
    void write_refresh(Frame& frame);
}

#endif

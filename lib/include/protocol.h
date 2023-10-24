#ifndef OWL_LIB_PROTOCOL
#define OWL_LIB_PROTOCOL

#include <cstdint>

namespace owl
{
    enum class Type : uint16_t
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
        uint16_t size;
    } __attribute__((__packed__));

    struct Feedback
    {
        int32_t temperature;
        int32_t humidity;
        int32_t focus_pos;
        enum State focus_state;
    } __attribute__((__packed__));

    struct gps
    {
        float latitude;
        float longitude;
        float altitude;
        int time;
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

    void write_trace(int fd, char const* message);
    void write_feedback(int fd, Feedback const& feeback);

    int readData(int fd, uint8_t* buffer, int size);
    template<typename T>
    int readData(int fd, T& data)
    {
        return readData(fd, reinterpret_cast<uint8_t*>(&data), sizeof(T));
    }
}

#endif

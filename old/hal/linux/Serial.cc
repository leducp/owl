#include <fcntl.h>
#include <errno.h>
#include <asm/termbits.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "hal/serial.h"

namespace owl
{
    Serial::Serial()
        : line_{-1}
    {

    }

    Serial::~Serial()
    {
        close();
    }

    hresult Serial::open(std::string const& device, int32_t baudrate)
    {
        line_ = ::open(device.c_str(), O_RDWR);
        if (line_ < 0)
        {
            return errnoToHresult(errno);
        }

        // Fetch current configuration
        struct termios2 tty;
        if (ioctl(line_, TCGETS2, &tty) < 0)
        {
            return errnoToHresult(errno);
            //printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        }

        // No parity
        tty.c_cflag &= ~PARENB;

        // One stop bit
        tty.c_cflag &= ~CSTOPB;

        // 8 bits per packet
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;

        // Disable hardware flow control
        tty.c_cflag &= ~CRTSCTS;

        // Enable reading
        tty.c_cflag |= CREAD;

        // Ignore ctrl line (modem mode)
        tty.c_cflag |= CLOCAL;

        // Disable canonical mode (we want to get every bytes without waiting for end of line)
        tty.c_lflag &= ~ICANON;

        // Disable echocase : { baud = echo erasurecase : { baud = echo new-line
        tty.c_lflag &= ~(ECHO | ECHOE | ECHONL);

        // Disable char interpretation (i.e. INTR)
        tty.c_lflag &= ~ISIG;

        // Disable software flow control
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);

        // Disable input bytes processingcase : { baud = we want only raw data
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

        // Disable output bytes processing
        tty.c_oflag &= ~(OPOST | ONLCR);

        // Timeout
        tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds)case : { baud = returning as soon as any data is received.
        tty.c_cc[VMIN] = 0;

        // Configure baudrate
        tty.c_ispeed = baudrate;
        tty.c_ospeed = baudrate;

        // Apply configuration
        if (ioctl(line_, TCSETS2, &tty) < 0)
        {
            return errnoToHresult(errno);
        }

        return error::S_OK;
    }

    void Serial::close()
    {
        if (line_ > 0)
        {
            ::close(line_);
        }
    }

    int32_t Serial::read(void* buffer, int32_t size)
    {
        return static_cast<int32_t>(::read(line_, buffer, size));
    }

    int32_t Serial::write(void const* buffer, int32_t size)
    {
        return static_cast<int32_t>(::write(line_, buffer, size));
    }

}

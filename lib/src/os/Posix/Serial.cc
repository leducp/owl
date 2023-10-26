#include "os/Serial.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
//#include <string.h>

namespace owl
{
    Serial::Serial()
        : line_{-1}
    {

    }

    Serial::~Serial()
    {
        if (line_ >= 0)
        {
            ::close(line_);
        }
    }


    int Serial::open(std::string const& device, int baudrate)
    {
        int termios_baudrate = 0;
        switch (baudrate)
        {
            case 2400:    { termios_baudrate = B2400;    break; }
            case 4800:    { termios_baudrate = B4800;    break; }
            case 9600:    { termios_baudrate = B9600;    break; }
            case 19200:   { termios_baudrate = B19200;   break; }
            case 38400:   { termios_baudrate = B38400;   break; }
            case 57600:   { termios_baudrate = B57600;   break; }
            case 115200:  { termios_baudrate = B115200;  break; }
            case 230400:  { termios_baudrate = B230400;  break; }
            case 460800:  { termios_baudrate = B460800;  break; }
            case 500000:  { termios_baudrate = B500000;  break; }
            case 576000:  { termios_baudrate = B576000;  break; }
            case 921600:  { termios_baudrate = B921600;  break; }
            case 1000000: { termios_baudrate = B1000000; break; }
            case 1152000: { termios_baudrate = B1152000; break; }
            case 1500000: { termios_baudrate = B1500000; break; }
            case 2000000: { termios_baudrate = B2000000; break; }
            case 2500000: { termios_baudrate = B2500000; break; }
            case 3000000: { termios_baudrate = B3000000; break; }
            case 3500000: { termios_baudrate = B3500000; break; }
            case 4000000: { termios_baudrate = B4000000; break; }
            default:
            {
                return -1;
            }
        }

        line_ = open(device.c_str(), O_RDWR | O_NOCTTY);
        if (line_ < 0)
        {
            return line_;
        }

        // Fetch current configuration
        struct termios tty;
        int rc = tcgetattr(line_, &tty);
        if (rc < 0)
        {
            return rc;
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

        // Baudrate
        rc = cfsetispeed(&tty, termios_baudrate);
        if (rc < 0)
        {
            return rc;
        }

        rc = cfsetospeed(&tty, termios_baudrate);
        if (rc < 0)
        {
            return rc;
        }

        return tcsetattr(line_, TCSAFLUSH, &tty);
    }

    void Serial::close()
    {

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

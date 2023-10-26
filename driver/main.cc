#include <sys/boardctl.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "protocol.h"
#include "FramePosix.h"
#include "actuators.h"
#include "sensors.h"
#include "gps.h"
#include "Time.h"

using namespace std::chrono;

extern "C"
{
    #include "rp2040_gpio.h"
}

void blink(int rep, nanoseconds delay)
{
    for (int i = 0; i < rep; ++i)
    {
        rp2040_gpio_put(25, true);
        owl::sleep(delay);
        rp2040_gpio_put(25, false);
        owl::sleep(delay);
    }
}

extern "C" int main(int argc, char* argv[])
{
    int ret = boardctl(BOARDIOC_INIT, 0);
    if (ret < 0)
    {
        perror("INIT\n");
        return ret;
    }

    struct boardioc_usbdev_ctrl_s ctrl;
    void *handle;
    ctrl.usbdev   = BOARDIOC_USBDEV_CDCACM;
    ctrl.action   = BOARDIOC_USBDEV_CONNECT;
    ctrl.instance = 0;
    ctrl.handle   = &handle;
    ret = boardctl(BOARDIOC_USBDEV_CONTROL, (uintptr_t)&ctrl);
    if (ret < 0)
    {
        perror("USBDEV\n");
        return ret;
    }

    int fd;
    while (fd = open("/dev/ttyACM0", O_RDWR) < 0)
    {
        blink(1, 5ms);
    }
    close(fd);
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NONBLOCK);
/*
    freopen("/dev/console", "w+", stdout);
    int flags = fcntl(STDOUT_FILENO, F_GETFL);
    fcntl(STDOUT_FILENO, F_SETFL, flags | O_NONBLOCK);
    owl::sleep(2s);
    printf("START! \n");
*/
    owl::init_gps();
    owl::init_sensors();
    owl::init_actuators();
    owl::FramePosix frame(fd);

    while (1)
    {
        owl::sleep(10ms);

        if (frame.read() == false)
        {
            continue;
        }
        blink(1, 500us);

        owl::Header const* header = reinterpret_cast<owl::Header const*>(frame.data());
        uint8_t const* payload = frame.data() + sizeof(owl::Header);

        switch (header->type)
        {
            case owl::Type::CONTROL:
            {
                owl::write_trace(frame, "ctrl");

                owl::Control const* control = reinterpret_cast<owl::Control const*>(payload);
                if (control->steps != 0)
                {
                    struct stepper_job_s job;
                    job.steps = control->steps;
                    job.speed = control->speed;
                    owl::push_task(job);
                    owl::write_trace(frame, "push");
                }

                break;
            }
            default:
            {

            }
        }
        frame.reset();

        owl::Feedback feedback;
        update(feedback);

        struct stepper_status_s stat = owl::status();
        feedback.focus_pos = stat.position;
        switch (stat.state)
        {
            case STEPPER_STATE_IDLE:  { feedback.focus_state = owl::State::READY; break; }
            case STEPPER_STATE_READY: { feedback.focus_state = owl::State::READY; break; }
            case STEPPER_STATE_RUN:   { feedback.focus_state = owl::State::BUSY;  break; }
            default:
            {
                feedback.focus_state = owl::State::FAULT;
            }
        }

        owl::write_feedback(frame, feedback);

        owl::GPS gps;
        update(gps);
        owl::write_gps(frame, gps);
    }

    return 0;
}

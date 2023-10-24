#include <sys/boardctl.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "protocol.h"
#include "worker.h"

extern "C"
{
    #include "rp2040_gpio.h"
}

void blink(int rep, int msec)
{
    for (int i = 0; i < rep; ++i)
    {
        rp2040_gpio_put(25, true);
        usleep(msec * 1000);
        rp2040_gpio_put(25, false);
        usleep(msec * 1000);
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
        usleep(1000);
    }
    close(fd);
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
    owl::init();

/*
    freopen("/dev/console", "w+", stdout);
    int flags = fcntl(STDOUT_FILENO, F_GETFL);
    fcntl(STDOUT_FILENO, F_SETFL, flags | O_NONBLOCK);
    printf("START! \n");
*/

    FramePosix frame(fd);

    while (1)
    {
        blink(1, 1);

        if (frame.read() == false)
        {
            continue;
        }

        owl::Header* header = reinterpret_cast<owl::Header*>(frame.data());
        uint8_t* payload = reinterpret_cast<owl::Header*>(frame.data() + sizeof(owl::Header));

        switch (header->type)
        {
            case owl::Type::CONTROL:
            {
                owl::Header yolo;
                yolo.type = owl::Type::TRACE;
                yolo.size = 4;
                ::write(fd, &yolo, 4);
                ::write(fd, "ctrl", 4);

                //owl::write_trace(fd, "ctrl");
                owl::Control* control = reinterpret_cast<owl::Control*>(buffer);
                if (control->steps != 0)
                {
                    struct stepper_job_s job;
                    job.steps = control->steps;
                    job.speed = control->speed;
                    owl::push_task(job);
                    //owl::write_trace(fd, "push");
                }

                break;
            }
            default:
            {

            }
        }

        struct stepper_status_s stat = owl::status();
        owl::Feedback feedback;
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

        owl::write_feedback(fd, feedback);

        tcflush(fd, TCIOFLUSH);
    }

    return 0;
}

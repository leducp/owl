#include <sys/boardctl.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <cmath>

#include "protocol.h"
#include "Time.h"
#include "FramePosix.h"

#include "focuser.h"
#include "gps.h"
#include "outputs.h"
#include "sensors.h"

using namespace std::chrono;

enum INIT_STATUS
{
    GPS,
    SENSORS,
    FOCUSER,
    OUTPUTS,
    END
};
#define LED_MAX 70
#define LED_MIN 1

uint8_t const glow[] =
{
    LED_MIN + 5,  LED_MIN + 7,  LED_MIN + 10, LED_MIN + 14, LED_MIN + 18,
    LED_MIN + 22, LED_MIN + 26, LED_MIN + 30, LED_MIN + 34, LED_MIN + 38,
    LED_MIN + 42, LED_MIN + 46, LED_MIN + 50, LED_MIN + 54, LED_MIN + 58,
    LED_MIN + 64, LED_MIN + 66, LED_MIN + 68, LED_MIN + 66, LED_MIN + 64,
    LED_MIN + 58, LED_MIN + 54, LED_MIN + 50, LED_MIN + 46, LED_MIN + 42,
    LED_MIN + 38, LED_MIN + 34, LED_MIN + 30, LED_MIN + 26, LED_MIN + 22,
    LED_MIN + 18, LED_MIN + 14, LED_MIN + 10, LED_MIN + 7,  LED_MIN + 5,
};
static int glow_pos = 0;

namespace owl
{
    void report_init(int status_report[4])
    {
        for (int i= 0; i < INIT_STATUS::END; ++i)
        {
            if (status_report[i] < 0)
            {
                set_output(LED_MAX, 0);
                sleep(300ms);
                set_output(0, 0);
                sleep(700ms);
            }
            else
            {
                set_output(LED_MAX, 0);
                sleep(700ms);
                set_output(0, 0);
                sleep(300ms);
            }
        }

        sleep(300ms);
    }

    void com_report(nanoseconds last_com)
    {
        static nanoseconds last = 0ns;

        if (elapsed_time(last_com) < 2s)
        {
            set_output(LED_MAX, 0);
            return;
        }

        if (elapsed_time(last) < 80ms)
        {
            return;
        }
        last = since_epoch();

        set_output(glow[glow_pos], 0);
        glow_pos++;
        if (glow_pos == 34)
        {
            glow_pos = 0;
        }
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
/*
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
    */

    /*
    freopen("/dev/console", "w+", stdout);
    int flags = fcntl(STDOUT_FILENO, F_GETFL);
    fcntl(STDOUT_FILENO, F_SETFL, flags | O_NONBLOCK);
    owl::sleep(2s);
    printf("START! \n");
    */

    owl::blink(10, 1ms);
    int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);

    int status_report[4];
    status_report[GPS]     = owl::init_gps();
    status_report[SENSORS] = owl::init_sensors();
    status_report[OUTPUTS] = owl::init_outputs();
    status_report[FOCUSER] = owl::init_focuser();
    owl::report_init(status_report);

    owl::FramePosix frame(fd);
    nanoseconds last = 0ns;

    while (1)
    {
        owl::sleep(10ms);
        owl::com_report(last);

        if (frame.read() == false)
        {
            continue;
        }
        owl::blink(1, 500us);
        last = owl::since_epoch();

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
        owl::update(feedback);

        struct stepper_status_s stat;
        owl::update(stat);
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
        owl::update(gps);
        owl::write_gps(frame, gps);
    }

    return 0;
}

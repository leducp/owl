#include <sys/boardctl.h>
#include "rp2040_gpio.h"
#include <nuttx/motor/stepper.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "stepper.h"

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

int main(int argc, char* argv[])
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
    while (ret = open("/dev/ttyACM0", O_WRONLY | O_NONBLOCK) < 0)
    {
        usleep(1000);
    }
    close(ret);

    freopen("/dev/console", "w+", stdout);
    int flags = fcntl(STDOUT_FILENO, F_GETFL);
    fcntl(STDOUT_FILENO, F_SETFL, flags | O_NONBLOCK);

    printf("START! \n");


    int stepper = init_stepper();
    struct stepper_status_s status;
    struct stepper_job_s job;
    job.speed = 5000;

    int const TURN = 32 * 200;

    while (1)
    {
        job.steps = TURN * 10;
        ret = write(stepper, &job, sizeof(struct stepper_job_s)); /* blocking */
        if (ret != sizeof(struct stepper_job_s))
        {
            printf("write: %s\n", strerror(errno));
        }

        job.steps = -TURN * 10;
        ret = write(stepper, &job, sizeof(struct stepper_job_s)); /* blocking */
        if (ret != sizeof(struct stepper_job_s))
        {
            printf("write: %s\n", strerror(errno));
        }

        printf("Check now!\n");
        usleep(1000 * 1000 * 4);
    }

    return 0;
}

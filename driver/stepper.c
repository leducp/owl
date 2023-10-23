#include <nuttx/motor/drv8825.h>
#include "rp2040_gpio.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include "stepper.h"

#define DIR     13
#define STEP    14
#define SLEEP   15

#define M0      18
#define M1      19
#define M2      20

#define RESET   21

void initialize(void)
{
    rp2040_gpio_init(DIR);
    rp2040_gpio_init(STEP);
    rp2040_gpio_init(SLEEP);

    rp2040_gpio_init(M0);
    rp2040_gpio_init(M1);
    rp2040_gpio_init(M2);

    rp2040_gpio_init(RESET);

    rp2040_gpio_setdir(DIR,   true);
    rp2040_gpio_setdir(STEP,  true);
    rp2040_gpio_setdir(SLEEP, true);

    rp2040_gpio_setdir(M0, true);
    rp2040_gpio_setdir(M1, true);
    rp2040_gpio_setdir(M2, true);

    rp2040_gpio_setdir(RESET, true);
}

void step(int level)
{
    rp2040_gpio_put(STEP, level);
}

void direction(int level)
{
    rp2040_gpio_put(DIR, level);
}

void microstepping(int ms1, int ms2, int ms3)
{
    printf("micro (%d %d %d)\n", ms1, ms2, ms3);
    rp2040_gpio_put(M0, ms1);
    rp2040_gpio_put(M1, ms2);
    rp2040_gpio_put(M2, ms3);
}

void enable(int level)
{
    printf("enable (%d)\n", level);
    rp2040_gpio_put(RESET, level);
}

void idle(int level)
{
    printf("idle (%d)\n", level);
    rp2040_gpio_put(SLEEP, !level);
}

int fault(void)
{
    return 0;
}

static struct drv8825_ops_s g_drv8825_ops =
{
    initialize,
    step,
    direction,
    microstepping,
    enable,
    idle,
    fault
};

int init_stepper(void)
{
    drv8825_register("/dev/focus", &g_drv8825_ops);
    int fd = open("/dev/focus", O_RDWR);
    int rc = ioctl(fd, STEPIOC_MICROSTEPPING, 32);
    if (rc < 0)
    {
        printf("STEPIOC_MICROSTEPPING: %s\n", strerror(errno));
    }

    rc = ioctl(fd, STEPIOC_IDLE, STEPPER_DISABLE_IDLE);
    if (rc < 0)
    {
        printf("STEPIOC_IDLE: %s\n", strerror(errno));
    }
    return fd;
}

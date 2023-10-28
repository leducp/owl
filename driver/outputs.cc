#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>

#include <nuttx/timers/pwm.h>

#include "outputs.h"
#include "os/Mutex.h"

extern "C"
{
    #include "rp2040_gpio.h"
}

namespace owl
{
    static pthread_t thread;
    static Mutex mutex{};
    static sem_t sem;
    static uint8_t outputs[8];

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

    static void* worker(void* arg)
    {
        // resolution = 0xffff / (freq / 125MHz)
        // -> 2000 with 62499
        struct pwm_info_s info[4];
        info[0].frequency = 62499;
        info[1].frequency = 62499;
        info[2].frequency = 62499;
        info[3].frequency = 62499;

        int pwms[4];
        pwms[0] = ::open("/dev/pwm0", O_WRONLY);
        pwms[1] = ::open("/dev/pwm1", O_WRONLY);
        pwms[2] = ::open("/dev/pwm3", O_WRONLY);
        pwms[3] = ::open("/dev/pwm4", O_WRONLY);

        // Map input to NuttX pwm_info_s structures
        ub16_t* duty[8];
        duty[0] = &info[0].channels[0].duty;
        duty[1] = &info[0].channels[1].duty;
        duty[2] = &info[1].channels[0].duty;
        duty[3] = &info[1].channels[1].duty;
        duty[4] = &info[2].channels[0].duty;
        duty[5] = &info[2].channels[1].duty;
        duty[6] = &info[3].channels[0].duty;
        duty[7] = &info[3].channels[1].duty;

        // Start PWMs
        for (int i = 0; i < 4; ++i)
        {
            ::ioctl(pwms[i], PWMIOC_SETCHARACTERISTICS, info + i);
            ::ioctl(pwms[i], PWMIOC_START, 0);
        }

        // Loop control
        while (true)
        {
            sem_wait(&sem);

            // Retrieve control
            {
                LockGuard guard(mutex);
                for (int i = 0; i < 8; ++i)
                {
                    *duty[i] = outputs[i];
                }
            }

            // Apply
            for (int i = 0; i < 4; ++i)
            {
                ::ioctl(pwms[i], PWMIOC_SETCHARACTERISTICS, info + i);
            }
        }

        return nullptr;
    }

    int init_outputs()
    {
        std::memset(outputs, 0, 8);
        sem_init(&sem, 0, 0);
        pthread_create(&thread, nullptr, worker, nullptr);
        return 0;
    }

    void set_output(uint8_t duty, int id)
    {
        {
            LockGuard guard(mutex);
            outputs[id] = duty;
        }
        sem_post(&sem);
    }
}

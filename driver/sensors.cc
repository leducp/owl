extern "C"
{
    #include <nuttx/sensors/sht3x.h>
    #include <nuttx/i2c/i2c_master.h>
}

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include "sensors.h"
#include "Time.h"
#include "os/Mutex.h"

namespace owl
{
    static pthread_t thread;
    static Mutex mutex{};

    static Feedback feedback;

    static void* worker(void* arg)
    {
        int fd = ::open("/dev/sht31", O_RDWR);
        if (fd < 0)
        {
            printf("oops sht32: %s\n", strerror(errno));
            feedback.sensor_state = State::FAULT;
            return nullptr;
        }

        int ret = ::ioctl(fd, SNIOC_START, nullptr);
        if (ret < 0)
        {
            printf("Error to run ioctl SNIOC_START (%s)\n", strerror(errno));
            feedback.sensor_state = State::FAULT;
            return nullptr;
        }

        while (true)
        {
            Feedback feedback_tmp;

            sht3x_meas_data_s data;
            ret = ioctl(fd, SNIOC_READ_CONVERT_DATA, &data);
            if (ret < 0)
            {
                feedback_tmp.sensor_state = State::FAULT;
            }
            else
            {
                feedback_tmp.sensor_state = State::READY;
                feedback_tmp.temperature = data.temperature;
                feedback_tmp.humidity    = data.humidity;
            }

            {
                LockGuard guard(mutex);
                feedback = feedback_tmp;
            }

            printf("temperature: %f\n", data.temperature);
            printf("humidity:    %f\n", data.humidity);

            sleep(100ms);
        }
    }

    int init_sensors()
    {
        file filep;
        file_open(&filep, "/dev/i2c1", O_RDWR);

        i2c_master_s* i2c = *static_cast<i2c_master_s**>(filep.f_inode->i_private);
        sht3x_register("/dev/sht31", i2c, 0x44);

        file_close(&filep);

        pthread_create(&thread, nullptr, worker, nullptr);
        return -1;
    }

    void update(Feedback& feedback_out)
    {
        LockGuard guard(mutex);
        feedback_out = feedback;
    }
}

#include <semaphore.h>
#include <pthread.h>
#include <list>

extern "C"
{
    #include "stepper.h"
}

#include "focuser.h"
#include "os/Mutex.h"

namespace owl
{
    static pthread_t thread;
    static sem_t sem;
    static Mutex mutex{};
    static stepper_status_s stepper_status{};
    static std::list<stepper_job_s> jobs{};
    static stepper_job_s test_job{};

    static void* worker(void* arg)
    {
        int stepper = init_stepper();

        while (true)
        {
            sem_wait(&sem);

            stepper_job_s job;
            {
                LockGuard guard(mutex);
                job = jobs.back();
                jobs.pop_back();
                stepper_status.state = STEPPER_STATE_RUN;
            }

            int ret = write(stepper, &job, sizeof(stepper_job_s)); /* blocking */
            if (ret != sizeof(stepper_job_s))
            {
                //TODO write TRACE
                //printf("write: %s\n", strerror(errno));
            }

            stepper_status_s status;
            ret = read(stepper, &status, sizeof(stepper_status_s));
            if (ret != sizeof(stepper_status_s))
            {
                //TODO write TRACE
                //printf("write: %s\n", strerror(errno));
            }

            {
                LockGuard guard(mutex);
                stepper_status = status;
            }
        }
    }

    int init_focuser()
    {
        sem_init(&sem, 0, 0);
        pthread_create(&thread, nullptr, worker, nullptr);
        return 0;
    }

    void push_task(stepper_job_s const& job)
    {
        {
            LockGuard guard(mutex);
            jobs.push_front(job);
        }

        sem_post(&sem);
    }

    void update(stepper_status_s& status)
    {
        {
            LockGuard guard(mutex);
            status = stepper_status;
        }
        sem_post(&sem);
    }
}

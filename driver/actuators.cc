#include <semaphore.h>
#include <pthread.h>
#include <queue>

extern "C"
{
    #include "stepper.h"
}

#include "actuators.h"

namespace owl
{
    static pthread_t thread;
    static sem_t sem;
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    static stepper_status_s stepper_status;
    static std::queue<stepper_job_s> jobs;

    static void* worker(void* arg)
    {
        int stepper = init_stepper();

        while (true)
        {
            sem_wait(&sem);

            pthread_mutex_lock(&mutex);
            stepper_job_s job = jobs.front();
            jobs.pop();
            stepper_status.state = STEPPER_STATE_RUN;
            pthread_mutex_unlock(&mutex);

            int ret = write(stepper, &job, sizeof(stepper_job_s)); /* blocking */
            if (ret != sizeof(stepper_job_s))
            {
                //TODO write TRACE
                //printf("write: %s\n", strerror(errno));
            }

            pthread_mutex_lock(&mutex);
            ret = read(stepper, &stepper_status, sizeof(stepper_status_s));
            pthread_mutex_unlock(&mutex);
            if (ret != sizeof(stepper_status_s))
            {
                //TODO write TRACE
                //printf("write: %s\n", strerror(errno));
            }
        }
    }

    void init_actuators()
    {
        sem_init(&sem, 0, 0);
        pthread_create(&thread, nullptr, worker, nullptr);
    }

    void push_task(stepper_job_s const& job)
    {
        pthread_mutex_lock(&mutex);
        jobs.push(job);
        pthread_mutex_unlock(&mutex);

        sem_post(&sem);
    }

    stepper_status_s status()
    {
        pthread_mutex_lock(&mutex);
        struct stepper_status_s stat = stepper_status;
        pthread_mutex_unlock(&mutex);

        return stat;
    }
}

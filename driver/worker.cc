#include <semaphore.h>
#include <pthread.h>

extern "C"
{
    #include "stepper.h"
}

#include "worker.h"

namespace owl
{
    pthread_t thread;
    sem_t sem;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
    struct stepper_status_s stepper_status;
    struct stepper_job_s next_job;

    void* worker(void* arg) 
    {
        int stepper = init_stepper();

        while (true)
        {
            sem_wait(&sem);

            pthread_mutex_lock(&mutex);
            struct stepper_job_s job = next_job;
            stepper_status.state = STEPPER_STATE_RUN;
            pthread_mutex_unlock(&mutex);

            int ret = write(stepper, &job, sizeof(struct stepper_job_s)); /* blocking */
            if (ret != sizeof(struct stepper_job_s))
            {
                //TODO write TRACE
                //printf("write: %s\n", strerror(errno));
            }

            pthread_mutex_lock(&mutex);
            ret = read(stepper, &stepper_status, sizeof(struct stepper_status_s));
            pthread_mutex_unlock(&mutex);
            if (ret != sizeof(struct stepper_status_s))
            {
                //TODO write TRACE
                //printf("write: %s\n", strerror(errno));
            }
        }
    }

    void init()
    {
        sem_init(&sem, 0, 0);
        pthread_create(&thread, nullptr, worker, nullptr);
    }

    void push_task(struct stepper_job_s const& job)
    {
        pthread_mutex_lock(&mutex);
        next_job = job;
        pthread_mutex_unlock(&mutex);

        sem_post(&sem);
    }

    struct stepper_status_s status()
    {
        pthread_mutex_lock(&mutex);
        struct stepper_status_s stat = stepper_status;
        pthread_mutex_unlock(&mutex);
        
        return stat;
    }
}

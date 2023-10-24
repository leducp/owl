#ifndef OWL_DRIVER_WORKER_H
#define OWL_DRIVER_WORKER_H

#include <nuttx/motor/stepper.h>

namespace owl
{
    void init();
    void push_task(struct stepper_job_s const& job);
    struct stepper_status_s status();
}

#endif

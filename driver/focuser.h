#ifndef OWL_DRIVER_FOCUSER_H
#define OWL_DRIVER_FOCUSER_H

#include <nuttx/motor/stepper.h>

namespace owl
{
    int init_focuser();
    void push_task(stepper_job_s const& job);
    void update(stepper_status_s& status);
}

#endif

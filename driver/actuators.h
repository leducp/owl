#ifndef OWL_DRIVER_ACTUATORS_H
#define OWL_DRIVER_ACTUATORS_H

#include <nuttx/motor/stepper.h>

namespace owl
{
    void init_actuators();
    void push_task(stepper_job_s const& job);
    stepper_status_s status();
}

#endif

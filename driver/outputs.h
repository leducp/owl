#ifndef OWL_DRIVER_OUTPUS_H
#define OWL_DRIVER_OUTPUS_H

#include "Time.h"

namespace owl
{
    void blink(int rep, nanoseconds delay);

    int init_outputs();

    // duty cycles
    void set_output(uint8_t duty, int id);
}

#endif

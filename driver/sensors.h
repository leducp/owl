#ifndef OWL_DRIVER_SENSORS_H
#define OWL_DRIVER_SENSORS_H

#include "protocol.h"

namespace owl
{
    int init_sensors();
    void update(Feedback& feedback);
}

#endif

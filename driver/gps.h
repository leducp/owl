#ifndef OWL_DRIVER_GPS_H
#define OWL_DRIVER_GPS_H

#include "protocol.h"

namespace owl
{
    void init_gps();
    void update(GPS& gps);
}

#endif

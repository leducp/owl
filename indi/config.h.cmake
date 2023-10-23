#ifndef OWL_CONFIG_H
#define OWL_CONFIG_H

namespace owl
{
    // Define INDI Data Dir 
    #cmakedefine INDI_DATA_DIR "@INDI_DATA_DIR@"
    // Driver version
    #define OWL_DRIVER_VERSION_MAJOR @OWL_DRIVER_VERSION_MAJOR@
    #define OWL_DRIVER_VERSION_MINOR @OWL_DRIVER_VERSION_MINOR@
}

#endif // OWL_CONFIG_H


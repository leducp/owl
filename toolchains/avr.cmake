set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_VERSION   1)
set(CMAKE_SYSTEM_PROCESSOR avr) 

set(CMAKE_C_COMPILER    avr-gcc)
set(CMAKE_CXX_COMPILER  avr-g++)
set(CMAKE_ASM_COMPILER  avr-gcc)

#include_directories(/usr/lib/avr/include)

add_definitions(-mmcu=atmega328p)
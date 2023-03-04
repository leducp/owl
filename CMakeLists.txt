cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(toolchains/pico_sdk_import.cmake)
include(toolchains/pico_extras_import_optional.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

set(OWL_SRCS    ${CMAKE_CURRENT_SOURCE_DIR}/actuators/stepper/AbstractStepper.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/actuators/stepper/A4988.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/actuators/stepper/ULN200x.cc

                ${CMAKE_CURRENT_SOURCE_DIR}/actuators/stepper/ULN200x.cc

                ${CMAKE_CURRENT_SOURCE_DIR}/sensors/SHT31.cc
                #${CMAKE_CURRENT_SOURCE_DIR}/common/Time.cc

                ${CMAKE_CURRENT_SOURCE_DIR}/hal/pico_pi/gpio.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/hal/pico_pi/i2c.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/hal/pico_pi/Time.cc

)

add_library(owl STATIC ${OWL_SRCS})
target_include_directories(owl PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/common)

target_link_libraries(owl pico_stdlib hardware_i2c)
target_include_directories(owl PUBLIC ${CMAKE_SOURCE_DIR}/hal/pico_pi)

add_executable(owl_example example_a4988.cc)
target_link_libraries(owl_example owl)

add_executable(owl_sht31 example_sht31.cc)
target_link_libraries(owl_sht31 owl hardware_i2c)
pico_enable_stdio_uart(owl_sht31 0)
pico_enable_stdio_usb(owl_sht31 1)
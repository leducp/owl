cmake_minimum_required(VERSION 3.12)

if (DEFINED PICOPI)
# Pull in SDK (must be before project)
include(toolchains/pico_sdk_import.cmake)
include(toolchains/pico_extras_import_optional.cmake)
endif()

project(owl C CXX ASM)
set(OWL_SRCS    ${CMAKE_CURRENT_SOURCE_DIR}/actuators/stepper/AbstractStepper.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/actuators/stepper/A4988.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/actuators/stepper/ULN200x.cc

                ${CMAKE_CURRENT_SOURCE_DIR}/sensors/SHT31.cc
)
add_compile_options(-Wall)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (DEFINED LINUX)
    set(OWL_SRCS_TARGET ${CMAKE_CURRENT_SOURCE_DIR}/hal/linux/Time.cc
                        ${CMAKE_CURRENT_SOURCE_DIR}/hal/linux/Serial.cc
                        ${CMAKE_CURRENT_SOURCE_DIR}/hal/linux/hardware_types.cc)
endif()

if (DEFINED PICOPI)
    if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
        message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
    endif()

    # Initialize the SDK
    pico_sdk_init()

    add_compile_options(
            -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
            -Wno-unused-function # we have some for the docs that aren't called
            -Wno-maybe-uninitialized
            )


    set(OWL_SRCS_TARGET ${CMAKE_CURRENT_SOURCE_DIR}/hal/pico_pi/gpio.cc
                        ${CMAKE_CURRENT_SOURCE_DIR}/hal/pico_pi/i2c.cc
                        ${CMAKE_CURRENT_SOURCE_DIR}/hal/pico_pi/Time.cc
    )
endif()

add_library(owl STATIC ${OWL_SRCS} ${OWL_SRCS_TARGET})
target_include_directories(owl PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/common)

if (DEFINED LINUX)
target_include_directories(owl PUBLIC ${CMAKE_SOURCE_DIR}/hal/linux)
add_executable(owl_gps example_neo8m.cc)
target_link_libraries(owl_gps owl)
endif()

if (DEFINED PICOPI)
    target_link_libraries(owl pico_stdlib hardware_i2c)
    target_include_directories(owl PUBLIC ${CMAKE_SOURCE_DIR}/hal/pico_pi)

    add_executable(owl_example example_a4988.cc)
    target_link_libraries(owl_example owl hardware_pwm)

    add_executable(owl_sht31 example_sht31.cc)
    target_link_libraries(owl_sht31 owl hardware_i2c)
    pico_enable_stdio_uart(owl_sht31 1)

    add_executable(owl_pwm example_pwm.cc)
    target_link_libraries(owl_pwm owl hardware_pwm)
    pico_enable_stdio_uart(owl_pwm 1)

    #pico_enable_stdio_usb(owl_sht31 1)
endif()
cmake_minimum_required(VERSION 3.16)

project(owl)

add_library(owl STATIC)
set_target_properties(owl PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSTION_INDEPENDENT_CODE ON)
target_include_directories(owl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(actuators)
add_subdirectory(sensors)


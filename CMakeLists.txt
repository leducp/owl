cmake_minimum_required(VERSION 3.12)
project(owl)

add_subdirectory(lib)

if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling: building driver.")
    add_subdirectory(driver)
else()
    message(STATUS "Host-compiling: build indi module")
    add_subdirectory(indi)
endif()

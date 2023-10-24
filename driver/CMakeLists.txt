cmake_minimum_required(VERSION 3.12)
project(owl)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib ${CMAKE_BINARY_DIR}/protocol})

add_executable(owl  ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stepper.c
    ${CMAKE_CURRENT_SOURCE_DIR}/worker.cc
    )
target_include_directories(owl PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(owl owl_protocol)

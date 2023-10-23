cmake_minimum_required(VERSION 3.12)
project(owl)

add_executable(owl  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
                    ${CMAKE_CURRENT_SOURCE_DIR}/stepper.c)
target_include_directories(owl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(owl
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stepper.c
    ${CMAKE_CURRENT_SOURCE_DIR}/actuators.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/sensors.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/gps.cc
    )
target_include_directories(owl PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(owl PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(owl owl_protocol)

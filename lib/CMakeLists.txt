add_library(owl_protocol STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Frame.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FramePosix.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FrameQueue.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Hash.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Time.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os/Mutex.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os/Posix/Mutex.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os/Posix/Serial.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os/Posix/Time.cc
    )
target_include_directories(owl_protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(owl_protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/os/Posix)
set_target_properties(owl_protocol PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
)

#add_executable(owl_test ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cc)
#target_link_libraries(owl_test owl_protocol)

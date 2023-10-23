cmake_minimum_required(VERSION 3.12)
project(owl_indi)

# Find INDI
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(INDI REQUIRED)

# Driver version
set(OWL_DRIVER_VERSION_MAJOR 0)
set(OWL_DRIVER_VERSION_MINOR 2)

# Create the driver xml file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/indi_owl.xml.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/indi_owl.xml
)

# Create config.h file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

# indi driver
add_executable(indi_owl ${CMAKE_CURRENT_SOURCE_DIR}/owl.cc)
target_include_directories(indi_owl PRIVATE ${INDI_INCLUDE_DIR} ${CMAKE_BINARY_DIR})
target_link_libraries(indi_owl ${INDI_LIBRARIES})

# tell cmake where to install our executable
install(TARGETS indi_owl RUNTIME DESTINATION bin)

# and where to put the driver's xml file.
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/indi_owl.xml
    DESTINATION ${INDI_DATA_DIR}
)

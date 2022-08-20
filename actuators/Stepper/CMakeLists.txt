set(STEPPER_SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractStepper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ULN200x.cc
    )


target_sources(owl PUBLIC ${STEPPER_SRCS})

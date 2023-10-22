set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_VERSION   1)
set(CMAKE_SYSTEM_PROCESSOR avr) 

set(CMAKE_C_COMPILER    avr-gcc)
set(CMAKE_CXX_COMPILER  avr-g++)
set(CMAKE_ASM_COMPILER  avr-gcc)

find_program(OBJCOPY avr-objcopy REQUIRED)
find_program(AVR_SIZE_TOOL avr-size REQUIRED)
find_program(FLASH_TOOL avrdude REQUIRED)

# Hook add_executable to generate flashing file (hex)
function(add_executablea target_name)
    # Call the original function
    _add_executable(${target_name} ${ARGN})
    
    # Generate hex file and print size
    add_custom_command(
        TARGET ${target_name} POST_BUILD
        COMMAND ${OBJCOPY} -O ihex ${target_name} ${target_name}.hex
        COMMAND ${AVR_SIZE_TOOL} --mcu=atmega328p --format=avr ${target_name}
    )

    # 
    add_custom_target(flash_${target_name}
        COMMAND ${FLASH_TOOL} -v -p atmega328p -c arduino -P /dev/ttyUSB0 -D -U flash:w:${target_name}.hex:i
        DEPENDS ${target_name}.hex
    )
endfunction()

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mmcu=atmega328p -DF_CPU=16000000UL -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -mrelax -fdata-sections -ffunction-sections")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=./ch.map,--cref,--gc-sections -Wl,-u,vfprintf -lprintf_min -Wl,-u,vfscanf -lscanf_min -lm")

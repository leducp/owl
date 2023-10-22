set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_VERSION   1)
set(CMAKE_SYSTEM_PROCESSOR arm) 

set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(OBJCOPY arm-none-eabi-objcopy REQUIRED)
find_program(SIZE_TOOL arm-none-eabi-size REQUIRED)
#find_program(FLASH_TOOL avrdude REQUIRED)

# Hook add_executable to generate flashing file (hex)
function(add_executablea target_name)
    # Call the original function
    _add_executable(${target_name} ${ARGN})
    
    # Generate hex file and print size
    add_custom_command(
        TARGET ${target_name} POST_BUILD
        COMMAND ${OBJCOPY} -O ihex ${target_name} ${target_name}.hex
        COMMAND ${OBJCOPY} -O binary ${target_name} ${target_name}.bin
        COMMAND ${SIZE_TOOL} ${target_name}
    )

    # 
    #add_custom_target(flash_${target_name}
    #    COMMAND ${FLASH_TOOL} -v -p atmega328p -c arduino -P /dev/ttyUSB0 -D -U flash:w:${target_name}.hex:i
    #    DEPENDS ${target_name}.hex
    #)
endfunction()

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mcpu=cortex-m4 -fomit-frame-pointer -falign-functions=16 -ffunction-sections -fdata-sections -fno-common -flto -MD -MP -MF ")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=./ch.map,--cref,--no-warn-mismatch,--library-path=/home/leduc/projet/ChibiOS_21.11.2/os/common/startup/ARMCMx/compilers/GCC/ld,--script=/home/leduc/projet/ChibiOS_21.11.2/os/common/startup/ARMCMx/compilers/GCC/ld/STM32F401xC.ld,--gc-sections,--defsym=__process_stack_size__=0x400,--defsym=__main_stack_size__=0x400")

# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk")
set(PICO_TOOLCHAIN_PATH "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi")
SET(CMAKE_C_COMPILER "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_CXX_COMPILER "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi/bin/arm-none-eabi-g++.exe")

# Include build functions from Pico SDK
include("C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk/external/pico_sdk_import.cmake")

# Set name of project (as PROJECT_NAME) and C/C   standards
project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)



# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

pico_find_compiler(PICO_COMPILER_LD ${PICO_GCC_TRIPLE}-ld)

function(add_resource target file)
  get_filename_component(NAME ${ARGV1} NAME_WE)
  set(FILENAME ${ARGV1})

  add_custom_command(
    OUTPUT ${NAME}.o

    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
            ${CMAKE_CURRENT_BINARY_DIR}

    COMMAND ${PICO_COMPILER_LD} -r -b binary -o ${NAME}.o ${FILENAME}
    DEPENDS ${FILENAME}
  )

  target_sources(${ARGV0} PRIVATE ${NAME}.o)
endfunction(add_resource)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
)

#$set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


add_subdirectory(common)
add_subdirectory(libraries)
add_subdirectory(drivers)



# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    pico_display_2_demo.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib 
    hardware_spi 
    hardware_pwm 
    hardware_dma 
    rgbled 
    button 
    pico_display_2 
    st7789 
    pico_graphics
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
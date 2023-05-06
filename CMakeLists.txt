# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

#include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
set(FREERTOS_CFG_DIRECTORY "/home/ndj/Programs/c-workspace/raspberry-pi-pico/config")
set(FREERTOS_SRC_DIRECTORY "/home/ndj/Programs/c-workspace/raspberry-pi-pico/pico/picoprobe/freertos")


# Set name of project (as PROJECT_NAME) and C/C++ Standards
project(multitask-pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Add FreeRTOS as a library
add_library(FreeRTOS STATIC
    ${FREERTOS_SRC_DIRECTORY}/event_groups.c
    ${FREERTOS_SRC_DIRECTORY}/list.c
    ${FREERTOS_SRC_DIRECTORY}/queue.c
    ${FREERTOS_SRC_DIRECTORY}/stream_buffer.c
    ${FREERTOS_SRC_DIRECTORY}/tasks.c
    ${FREERTOS_SRC_DIRECTORY}/timers.c
    ${FREERTOS_SRC_DIRECTORY}/portable/MemMang/heap_3.c
    ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0/port.c
)

# Build FreeRTOS
target_include_directories(FreeRTOS PUBLIC
    ${FREERTOS_CFG_DIRECTORY}/
    ${FREERTOS_SRC_DIRECTORY}/include
    ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0
)

# point out the CMake, where to find the executable source file
add_executable(${PROJECT_NAME}
        main.c
)
# create map/bin/hex/uf2 files.
pico_add_extra_outputs(${PROJECT_NAME})

# Pull in our pico_stdlib which pulls in commonly used features (gpio, timer-delay etc)
target_link_libraries(${PROJECT_NAME}
            pico_stdlib
            FreeRTOS
)

# To monitor through serial port disable UART communication and enable USB communication
# for the given project as below.
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)



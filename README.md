# MULTI-TASKING EXAMPLE FOR RASPBERRY-PI PICO

## CMAKE EXPLAINED
1. Build function for pico sdk is included first.
    ```cmake
    include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
    ```
2. Then path to FreeRTOSConfig.h and freertos source directory is provided.
    ```cmake
    set(FREERTOS_CFG_DIRECTORY "<path to FreeRTOSConfig.h>")
    set(FREERTOS_SRC_DIRECTORY "<path to freertos source directory>")
    ```
3. Initialize pico sdk. This will create a pico-sdk subdirectory in our project for the libraries.
    ```cmake
    pico_sdk_init()
    ```
4. Add freeRTOS as a library.
    ```cmake
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
    ```
5. Build freeRTOS
    ```cmake
    target_include_directories(FreeRTOS PUBLIC
        ${FREERTOS_CFG_DIRECTORY}/
        ${FREERTOS_SRC_DIRECTORY}/include
        ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0
    )
    ```
6. Since .uf2 file is used rather than binary file, all (map/bin/hex/uf2) files should be generated.
    ```cmake
    pico_add_extra_outputs(${PROJECT_NAME})
    ```
7. Link pico_stdlib and freeRTOS to the project.
    ```cmake
    target_link_libraries(${PROJECT_NAME}
                pico_stdlib
                FreeRTOS
    )
    ```
8. To monitor through serial port disable UART communication and enable USB communication for the given project as below.
    ```cmake
    pico_enable_stdio_usb(${PROJECT_NAME} 1)
    pico_enable_stdio_uart(${PROJECT_NAME} 0)
    ```
## CODE EXPLAINED
1. Include standard library for RPI-pico, stdio, freertos configurations and the task header from freertos.

    ***It is mandatory to include the FreeRTOS.h header file before any other FreeRTOS header files in your program. This is because the FreeRTOS.h header file includes the necessary definitions, structures, and functions that are required by all other FreeRTOS header files.***
    ```c
    #include "pico/stdlib.h"
    #include "FreeRTOS.h"
    #include "task.h"
    #include <stdio.h>
    ```
2. Initialize stdio functionalities for RPI-pico
    ```c
    stdio_init_all();
    ```
3. Define two functions that will be our two tasks.
    ```c
    static void task_1(void* params)
    {
        while(true)
        {
            printf("First\n");
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }   

    static void task_2(void* params)
    {
        while(true)
        {
            printf("Second\n");
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
    ```
4. Create tasks using **xTaskCreate()** function.
    - The function passed as task should be of void type and take a void pointer as an argument.
    - The stack size should be in bytes and in general 128 bytes is enough. But depends on the task itself.
    ```c 
    xTaskCreate(task_1, "task_1", 256, NULL, 1, NULL);
    xTaskCreate(task_2, "task_2", 256, NULL, 1, NULL);
    ```
5. The **vTaskStartScheduler()** function call is mandatory when using FreeRTOS on Raspberry Pi Pico or any other platform that supports FreeRTOS. It starts the FreeRTOS scheduler, which is responsible for managing the execution of tasks and other FreeRTOS objects. Without starting the scheduler, the tasks that are created with xTaskCreate() will not run.
    ```c
    vTaskStartScheduler();
    ```
6. Within an empty infinite while loop is placed at the end, which is optional.
    ```c
    while (true) {}
    ```
## COMPILING AND FLASHING 
1. Create a build direcotry in the project folder and cd into it.
    ```bash
    mkdir build && cd build
    ```
2. Execute cmake and run make command
    ```bash
    cmake ..
    make
    ```
3. After that connect the RPI-pico to the computer. 
   - If it is very first flashing direction connection to the computer will work.
   - If another firmware is already running, push **BOOTSEL** button and then plug the USB to the computer.
5. The RPI-pico is conneced to the computer as a drive named **RPI-RP2**. Copy the .uf2 file within in the build directory and paste it in **RPI-RP2**. Then the RPI-pico will automatically reboot and LED starts blinking.
6. For monitoring through serial interface, any program or software that has serial monitro functionality will work. For linux following command will work as well:
    ```bash
    cat /dev/<port name it RPI-pico is connected>
    ```

## THANK YOU
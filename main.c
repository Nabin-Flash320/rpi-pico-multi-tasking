#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>


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

void main()
{
    stdio_init_all();
    xTaskCreate(task_1, "task_1", 256, NULL, 1, NULL);
    xTaskCreate(task_2, "task_2", 256, NULL, 1, NULL);    

    // Start FreeRTOS task scheduler.
    vTaskStartScheduler();

    while(1){}
}

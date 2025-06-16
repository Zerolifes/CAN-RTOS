#ifndef NODE1_H
#define NODE1_H

#include "COMMON.h"
#include "OLED.h"
#include "CAN.h"
#include "HCSR04.h"

void task_hcsr04(void *args)
{
    uint8_t *dist = (uint8_t *)args;
    while (1)
    {
        uint32_t d = hcsr04_read();
        *dist = d > 255 ? 255 : d;
        vTaskDelay(pdMS_TO_TICKS(SYSTEM_DELAY_MS));
    }
}

void task_oled(void *args)
{
    uint8_t *dist = (uint8_t *)args;
    auto OLED = get_oled();
    wchar_t text[32];

    while (1)
    {
        OLED.clear();
        swprintf(text, 32, L"Dis: %ld cm", *dist);
        display(&OLED, text, 2, 0);
        vTaskDelay(pdMS_TO_TICKS(SYSTEM_DELAY_MS));
    }
}

void task_can(void *args)
{
    uint8_t *dist = (uint8_t *)args;
    uint8_t data[8] = {0};

    while (1)
    {
        data[0] = *dist;
        can_send(0x123, data);
        vTaskDelay(pdMS_TO_TICKS(SYSTEM_DELAY_MS));
    }
}

void node1_setup()
{
    clock_setup();
    oled_setup();
    can_setup();
    hcsr04_setup();
}

void node1_main()
{
    node1_setup();

    static uint8_t distance = 0;

    xTaskCreate(task_hcsr04, "HCSR04", 128, &distance, 3, NULL);
    xTaskCreate(task_oled,   "OLED",  256, &distance, 1, NULL);
    xTaskCreate(task_can,    "CAN",   128, &distance, 2, NULL);

    vTaskStartScheduler();
}

#endif

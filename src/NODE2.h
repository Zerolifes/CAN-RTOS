#ifndef NODE2_H
#define NODE2_H

#include "COMMON.h"
#include "OLED.h"
#include "CAN.h"
#include "TIME.h"

void node2_setup()
{
    clock_setup();
    oled_setup();
    can_setup();
}

static uint8_t distance_raw = 0;
static uint8_t speed_percent = 0;
static uint8_t send_ok = 0;

void task_recv_can(void *args)
{
    (void)args;
    while (1)
    {
        uint64_t rx_data = can_recv(0x123);
        if (rx_data != (uint64_t)-1)
        {
            distance_raw = rx_data & 0xFF;
        }
        vTaskDelay(pdMS_TO_TICKS(SYSTEM_DELAY_MS));
    }
}

void task_calc_speed(void *args)
{
    (void)args;
    while (1)
    {
        if (distance_raw >= 60)
            speed_percent = 100;
        else
            speed_percent = (distance_raw * 100) / 60;
        vTaskDelay(pdMS_TO_TICKS(SYSTEM_DELAY_MS));
    }
}

void task_send_speed(void *args)
{
    (void)args;
    uint8_t data[8] = {0};
    while (1)
    {
        data[0] = speed_percent;
        int res = can_send(0x124, data);
        send_ok = (res != -1) ? 1 : 0;
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void task_oled(void *args)
{
    (void)args;
    auto OLED = get_oled();
    wchar_t text[32];

    while (1)
    {
        OLED.clear();
        swprintf(text, 32, L"Dis: %d cm", distance_raw);
        display(&OLED, text, 2, 0);

        swprintf(text, 32, L"Speed: %d percent", speed_percent);
        display(&OLED, text, 3, 0);

        if (send_ok)
            swprintf(text, 32, L"CAN: OK");
        else
            swprintf(text, 32, L"CAN: FAIL");
        display(&OLED, text, 4, 0);

        vTaskDelay(pdMS_TO_TICKS(SYSTEM_DELAY_MS));
    }
}

void node2_main()
{
    node2_setup();

    xTaskCreate(task_recv_can,   "CANRecv", 128, NULL, 2, NULL);
    xTaskCreate(task_calc_speed, "Calc",    128, NULL, 1, NULL);
    xTaskCreate(task_send_speed, "CANTx",   128, NULL, 1, NULL);
    xTaskCreate(task_oled,       "OLED",    256, NULL, 1, NULL);

    vTaskStartScheduler();
}

#endif

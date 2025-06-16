#ifndef NODE3_H
#define NODE3_H

#include "COMMON.h"
#include "CAN.h"
#include "TIME.h"

void node3_setup()
{
    clock_setup();
    pwm_setup();
    can_setup();
}

static uint8_t speed_percent_3 = 0;

void task_can_receive(void *args)
{
    (void)args;
    while (1)
    {
        uint64_t rx_data = can_recv(0x124);
        if (rx_data != (uint64_t)-1)
        {
            speed_percent_3 = rx_data & 0xFF;
        }
        vTaskDelay(SYSTEM_DELAY_MS);
    }
}

void motor_control(void *args)
{
    (void)args;
    while (1)
    {
        uint32_t duty = (speed_percent_3 * 1000) / 100;
        timer_set_oc_value(TIM2, TIM_OC1, duty);
        vTaskDelay(SYSTEM_DELAY_MS);
    }
}

void node3_main()
{
    node3_setup();

    xTaskCreate(task_can_receive, "CANRecv", 128, NULL, 2, NULL);
    xTaskCreate(motor_control,   "Motor",   128, NULL, 1, NULL);

    vTaskStartScheduler();
}

#endif

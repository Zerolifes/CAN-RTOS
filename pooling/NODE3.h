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

void node3_main()
{
    node3_setup();
    uint8_t data[8];
    while (1)
    {
        uint64_t rx_data = can_recv(0x123);
        if (rx_data == -1) continue;
        for (int i = 0; i < 8; i++) 
        {
            data[i] = (rx_data >> (i * 8)) & 0xFF;
        }
        uint32_t distance = data[0]; 
        uint32_t duty_cycle = distance * 1000 / 50;
        timer_set_oc_value(TIM2, TIM_OC1, duty_cycle);
        delay_ms(200);
    }
}

#endif
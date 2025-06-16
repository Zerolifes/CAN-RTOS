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

void node2_main()
{
    node2_setup();
    auto OLED = get_oled();
    wchar_t text[32];
    uint8_t data[8];
    while (1)
    {
        OLED.clear();
        uint64_t rx_data = can_recv(0x123);
        for (int i = 0; i < 8; i++) 
        {
            data[i] = (rx_data >> (i * 8)) & 0xFF;
        }
        swprintf(text, 32, L"Dis: %ld cm", data[0]); 
        display(&OLED, text, 2, 0);
        delay_ms(500);
    }
}

#endif
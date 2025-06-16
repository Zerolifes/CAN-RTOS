#ifndef NODE1_H
#define NODE1_H

#include "COMMON.h"
#include "OLED.h"
#include "CAN.h"
#include "TIME.h"
#include "HCSR04.h"

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
    auto OLED = get_oled();
    wchar_t text[32];
    uint8_t data[8] = {0,0,0,0,0,0,0,0};
    while (1)
    {
        OLED.clear();
        uint32_t dis = hcsr04_read();
        data[0] = dis > 255 ? 255 : dis;
        int res = can_send(0x123, data);
        if (res!=-1)
        {
            swprintf(text, 32, L"sent: ok"); 
            display(&OLED, text, 3, 0);
        }
        else
        {
            swprintf(text, 32, L"sent: fail"); 
            display(&OLED, text, 3, 0);
        }
        swprintf(text, 32, L"Dis: %ld cm", data[0]); 
        display(&OLED, text, 2, 0);
        delay_ms(500);
    }
}

#endif
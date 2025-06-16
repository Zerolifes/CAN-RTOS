#ifndef HCSR04_H
#define HCSR04_H

#include "COMMON.h"
#include "TIME.h"

#define GPIO_TRIG GPIO0
#define GPIO_ECHO GPIO1

const uint32_t C_hcsr04 = 1715 * 5 / 3;

void hcsr04_setup(void) 
{
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_TRIG);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_ECHO);
    gpio_clear(GPIOA, GPIO_TRIG);
}

uint32_t hcsr04_read(void) 
{
    gpio_clear(GPIOA, GPIO_TRIG); 
    delay_us(2);              
    gpio_set(GPIOA, GPIO_TRIG);   
    delay_us(10);            
    gpio_clear(GPIOA, GPIO_TRIG); 

    uint32_t timeout = 50000; 
    while (!gpio_get(GPIOA, GPIO_ECHO) && timeout--);
    if (timeout == 0) return 0;

    uint32_t time = 0;
    timeout = 50000;
    while (gpio_get(GPIOA, GPIO_ECHO) && timeout--) 
    {
        time++;
        delay_us(1);
    }
    if (timeout == 0) return 0; 

    uint32_t distance = (time * C_hcsr04) / 100000;
    return distance;
}

#endif
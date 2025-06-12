#ifndef COMMON_H
#define COMMON_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/i2c.h>
#include <cwchar>
#include <stddef.h>
#include "OLED.h"

void init_gpio()
{
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOB);
}

void init_config()
{
    init_gpio();
    i2c_setup();
}


#endif
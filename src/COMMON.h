#ifndef COMMON_H
#define COMMON_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/timer.h>
#include <cwchar>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void clock_setup()
{
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}


#endif
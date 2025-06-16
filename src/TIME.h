#ifndef TIME_H
#define TIME_H

#include "COMMON.h"
#define SYSTEM_DELAY_MS 10


#define NOP_PER_MS 6150

void pwm_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_TIM2);

    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO0);

    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 72 - 1); 
    timer_set_period(TIM2, 1000 - 1);

    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
    timer_enable_oc_output(TIM2, TIM_OC1);
    timer_set_oc_polarity_high(TIM2, TIM_OC1); 
    timer_set_oc_value(TIM2, TIM_OC1, 0); 

    timer_enable_counter(TIM2);
}

void delay_ms(uint32_t ms) 
{
    uint32_t nops = ms * NOP_PER_MS;
    for (volatile uint32_t i = 0; i < nops; i++) 
    {
        __asm__("nop");
    }
}

void delay_us(uint32_t us) 
{
    uint32_t nops = us * NOP_PER_MS / 1000;
    for (volatile uint32_t i = 0; i < nops; i++) 
    {
        __asm__("nop");
    }
}


#endif
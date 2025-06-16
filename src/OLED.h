/*
    Zerolife
    Hướng dẫn sử dụng:
    hw::SSD1306 OLED = get_oled();
    wchar_t text[32];
    wchar_t other[] = L"abc";
    swprintf(text, sizeof(text)/sizeof(wchar_t), L"... %ls %d", other, int);
    display(&OLED, text, row, col);
 */

#ifndef OLED_H
#define OLED_H
#include "COMMON.h" 
#include "SSD1306.h"
#include "Fonts/Pentacom.h"

#define OLED_ADDR 0x3C

void oled_setup() 
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C1_SCL | GPIO_I2C1_SDA);
    i2c_peripheral_disable(I2C1);
    i2c_set_clock_frequency(I2C1, 36);  
    i2c_set_trise(I2C1, 37);            
    i2c_set_ccr(I2C1, 180);            
    i2c_set_own_7bit_slave_address(I2C1, 0x32); 
    i2c_peripheral_enable(I2C1);
}

hw::SSD1306 get_oled()
{
    static hw::SSD1306 oled(I2C1, OLED_ADDR, 128, 64);
    oled.init();
    oled.clear();
    return oled;
}

void display(hw::SSD1306 *oled, wchar_t *text, int row, int col) 
{
    row*=8;
    col*=8;
    hw::Fonts::Pentacom font;
    oled->drawWString(col, row, &font, hw::SSD1306::Color::White, hw::SSD1306::WrapType::NoWrap, text);
    oled->refresh();
}

#endif
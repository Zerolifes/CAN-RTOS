// #include <libopencm3/stm32/rcc.h>
// #include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/can.h>
// #include <stddef.h>

// // Định nghĩa nếu thiếu trong lib
// #define CAN_RF0R_FMP_MASK (0x3 << 0)

// static void clock_setup(void) {
//     rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]); // 72MHz
//     rcc_periph_clock_enable(RCC_GPIOA);
//     rcc_periph_clock_enable(RCC_GPIOB);
//     rcc_periph_clock_enable(RCC_GPIOC);
//     rcc_periph_clock_enable(RCC_AFIO);
//     rcc_periph_clock_enable(RCC_CAN1);
// }

// static void gpio_setup(void) {
//     gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
//                   GPIO_CNF_OUTPUT_PUSHPULL, GPIO13); // LED PC13
//     gpio_set(GPIOC, GPIO13);

//     gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ,
//                   GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);  // LED PB8
//     gpio_clear(GPIOB, GPIO8);

//     gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ,
//                   GPIO_CNF_OUTPUT_PUSHPULL, GPIO9);  // LED PB9
//     gpio_clear(GPIOB, GPIO9);
// }

// static void can_setup(void) {
//     can_reset(CAN1);
//     can_init(CAN1, true,  // loopback mode
//              true, false, false, false, false,
//              CAN_BTR_SJW_1TQ, CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ,
//              24, true, false); // 125kbps = 24MHz / (1+13+2) / 24

//     can_filter_id_mask_32bit_init(0, 0, 0, 0, true);
//     // can_filter_id_mask_32bit_init(0, 0x123 << 2, 0x7FF << 2, 0, true);
// }

// static void delay_ms(uint32_t ms) {
//     for (uint32_t i = 0; i < ms * 8000; i++) {
//         __asm__("nop");
//     }
// }

// int main(void) {
//     clock_setup();
//     gpio_setup();
//     can_setup();

//     // Nhấp nháy báo khởi động
//     for (int i = 0; i < 4; i++) {
//         gpio_toggle(GPIOC, GPIO13);
//         delay_ms(200);
//     }

//     uint8_t tx_data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
//     uint8_t rx_data[8];
//     uint32_t id;
//     bool ext, rtr;
//     uint8_t fmi, len;

//     while (1) {
//         // Gửi CAN
//         int result = can_transmit(CAN1, 0x123, false, false, 8, tx_data);
//         if (result >= 0) {
//             // Gửi thành công → LED PB8 nháy
//             gpio_toggle(GPIOB, GPIO8);
//         }
//         delay_ms(500);

//         // Nhận CAN bằng polling
//         if ((CAN_RF0R(CAN1) & CAN_RF0R_FMP_MASK) != 0) {
//             can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &len, rx_data, NULL);
//             // Nhận thành công → LED PB9 nháy
//             gpio_toggle(GPIOB, GPIO9);
//         }

//         // LED PC13 nháy để báo loop
//         gpio_toggle(GPIOC, GPIO13);
//         delay_ms(1000);
//     }

//     return 0;
// }

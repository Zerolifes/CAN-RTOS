// #include "COMMON.h"
// #include "OLED.h"

// void delay_ms(uint32_t ms) {
//     uint32_t cycles = ms * 72000 / 8; // Dựa trên xung nhịp 72 MHz
//     for (uint32_t i = 0; i < cycles; i++) {
//         __asm__("nop");
//     }
// }

// void can_setup_receiver(void) {
//     // Bật xung nhịp cho GPIOB, AFIO, và CAN1
//     rcc_periph_clock_enable(RCC_GPIOB);
//     rcc_periph_clock_enable(RCC_AFIO);
//     rcc_periph_clock_enable(RCC_CAN1);

//     // Remap CAN1 sang PB8 (RX) và PB9 (TX)
//     AFIO_MAPR |= AFIO_MAPR_CAN1_REMAP_PORTB;

//     // Cấu hình GPIO
//     gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
//                   GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9); // TX
//     gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
//                   GPIO_CNF_INPUT_PULL_UPDOWN, GPIO8);     // RX
//     gpio_set(GPIOB, GPIO8); // Kéo lên cho RX

//     // Reset CAN
//     can_reset(CAN1);

//     // Khởi tạo CAN với bitrate ~10 kbps
//     can_init(CAN1,
//              true,   // ttcm
//              false,  // abom
//              false,  // awum
//              false,  // nart
//              false,  // rflm
//              false,  // txfp
//              CAN_BTR_SJW_1TQ,  // Resync jump width: 1 TQ
//              CAN_BTR_TS1_13TQ, // Time segment 1: 13 TQ
//              CAN_BTR_TS2_2TQ,  // Time segment 2: 2 TQ
//              212,    // Prescaler cho ~10 kbps
//              false,  // loopback
//              false); // silent

//     // Cấu hình bộ lọc chỉ nhận ID 0x123
//     can_filter_init(0, true, false, 0x123 << 5, 0x7FF << 5, 0, true);
// }

// int main() {
//     // Khởi tạo hệ thống
//     init_config();
//     hw::SSD1306 OLED = get_oled();
//     wchar_t text[32];
//     OLED.clear();

//     // Hiển thị thông báo khởi động
//     swprintf(text, 32, L"RECEIVER: CAN TEST");
//     cout(&OLED, text, 0, 0);

//     delay_ms(1000);
//     can_setup_receiver();

//     uint8_t rx_data[8];
//     uint32_t rx_id;
//     bool ext, rtr;
//     uint8_t fmi, dlc;

//     while (1) {
//         // Xóa OLED để hiển thị mới
//         OLED.clear();

//         // Kiểm tra số tin nhắn trong FIFO 0
//         uint32_t fmp0 = (CAN_RF0R(CAN1) & CAN_RF0R_FMP0_MASK);
//         if (fmp0 > 0) {
//             // Nhận tin nhắn từ FIFO 0
//             can_receive(CAN1, 0, true, &rx_id, &ext, &rtr, &fmi, &dlc, rx_data, NULL);
//             swprintf(text, 32, L"RECV %lc %lc ID:%03x", rx_data[0], rx_data[1], rx_id);
//             cout(&OLED, text, 0, 0);
//         } else {
//             swprintf(text, 32, L"NO MSG RECEIVED");
//             cout(&OLED, text, 0, 0);
//         }

//         // Hiển thị Last Error Code (LEC)
//         uint32_t lec = (CAN_ESR(CAN1) & CAN_ESR_LEC_MASK) >> 4;
//         swprintf(text, 32, L"LEC: %d", lec);
//         cout(&OLED, text, 1, 0);

//         // Hiển thị Receive FIFO 0 Register (RFR0)
//         uint32_t rfr0 = CAN_RF0R(CAN1);
//         swprintf(text, 32, L"RFR0: %08lx", rfr0);
//         cout(&OLED, text, 2, 0);

//         // Hiển thị Error Status Register (ESR)
//         uint32_t esr = CAN_ESR(CAN1);
//         swprintf(text, 32, L"ESR: %08lx", esr);
//         cout(&OLED, text, 3, 0);

//         delay_ms(1000); // Cập nhật mỗi giây
//     }

//     return 0;
// }
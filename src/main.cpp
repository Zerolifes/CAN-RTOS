#include "COMMON.h"
#include "OLED.h"

void delay_ms(uint32_t ms) {
    uint32_t cycles = ms * 72000 / 8; // Dựa trên xung nhịp 72 MHz
    for (uint32_t i = 0; i < cycles; i++) {
        __asm__("nop");
    }
}

void can_setup_sender(void) {
    // Bật xung nhịp cho GPIOB, AFIO, và CAN1
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_CAN1);

    // Remap CAN1 sang PB8 (RX) và PB9 (TX)
    AFIO_MAPR |= AFIO_MAPR_CAN1_REMAP_PORTB;

    // Cấu hình GPIO
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9); // TX
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO8);     // RX
    gpio_set(GPIOB, GPIO8); // Kéo lên cho RX

    // Reset CAN
    can_reset(CAN1);

    // Khởi tạo CAN với bitrate ~10 kbps
    can_init(CAN1,
             true,   // ttcm
             false,  // abom
             false,  // awum
             false,  // nart (bật tự động retransmit)
             false,  // rflm
             false,  // txfp
             CAN_BTR_SJW_1TQ,  // Resync jump width: 1 TQ
             CAN_BTR_TS1_13TQ, // Time segment 1: 13 TQ
             CAN_BTR_TS2_2TQ,  // Time segment 2: 2 TQ
             212,    // Prescaler cho ~10 kbps
             false,  // loopback
             false); // silent

    // Cấu hình bộ lọc chấp nhận tất cả tin nhắn
    can_filter_init(0, true, false, 0, 0, 0, true);
}

int main() {
    // Khởi tạo hệ thống
    init_config();
    hw::SSD1306 OLED = get_oled();
    wchar_t text[32];
    OLED.clear();

    // Hiển thị thông báo khởi động
    swprintf(text, 32, L"SENDER: CAN TEST");
    cout(&OLED, text, 0, 0);

    delay_ms(1000);
    can_setup_sender();

    // Dữ liệu gửi
    uint8_t tx_data[8] = {'H', 'E', 'L', 'L', 'O', '!', '!', '\0'};

    while (1) {
        // Gửi tin nhắn CAN với ID 0x123
        int result = can_transmit(CAN1, 0x123, false, false, 8, tx_data);

        // Nếu mailbox bận, hủy yêu cầu truyền để giải phóng
        if (result == -1) {
            CAN_TSR(CAN1) |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
        }

        // Xóa OLED để hiển thị mới
        OLED.clear();

        // Hiển thị dữ liệu gửi và kết quả truyền
        swprintf(text, 32, L"SEND %lc %lc (%d)", tx_data[0], tx_data[1], result);
        cout(&OLED, text, 0, 0);

        // Hiển thị Last Error Code (LEC)
        uint32_t lec = (CAN_ESR(CAN1) & CAN_ESR_LEC_MASK) >> 4;
        swprintf(text, 32, L"LEC: %d", lec);
        cout(&OLED, text, 1, 0);

        // Hiển thị Transmit Status Register (TSR)
        uint32_t tsr = CAN_TSR(CAN1);
        swprintf(text, 32, L"TSR: %08lx", tsr);
        cout(&OLED, text, 2, 0);

        // Hiển thị Error Status Register (ESR) và kiểm tra bus-off
        uint32_t esr = CAN_ESR(CAN1);
        if (esr & CAN_ESR_BOFF) {
            swprintf(text, 32, L"BUS-OFF DETECTED");
            cout(&OLED, text, 3, 0);
            can_reset(CAN1); // Reset CAN nếu bus-off
            can_setup_sender(); // Cấu hình lại CAN
        } else {
            swprintf(text, 32, L"ESR: %08lx", esr);
            cout(&OLED, text, 3, 0);
        }

        delay_ms(1000); // Gửi mỗi giây
    }

    return 0;
}
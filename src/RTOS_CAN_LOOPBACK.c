// #include <libopencm3/stm32/rcc.h>
// #include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/can.h>
// #include "FreeRTOS.h"
// #include "task.h"

// #define CAN_RF0R_FMP_MASK (0x3 << 0)

// static void clock_setup(void) {
//     rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
//     rcc_periph_clock_enable(RCC_GPIOB);
//     rcc_periph_clock_enable(RCC_GPIOC);
//     rcc_periph_clock_enable(RCC_AFIO);
//     rcc_periph_clock_enable(RCC_CAN1);
// }

// static void gpio_setup(void) {
//     // LED PC13
//     gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
//     gpio_set(GPIOC, GPIO13);

//     // LED PB8, PB9
//     gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8 | GPIO9);
//     gpio_clear(GPIOB, GPIO8);
//     gpio_clear(GPIOB, GPIO9);
// }

// static void can_setup(void) {
//     can_reset(CAN1);
//     can_init(CAN1, true,  // loopback mode
//              true, false, false, false, false,
//              CAN_BTR_SJW_1TQ, CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ,
//              24, true, false);

//     can_filter_id_mask_32bit_init(0, 0, 0, 0, true);
// }

// static void task_can_tx(void *args) {
//     (void)args;
//     uint8_t tx_data[8] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
//     while (1) {
//         int res = can_transmit(CAN1, 0x123, false, false, 8, tx_data);
//         if (res >= 0) {
//             gpio_toggle(GPIOB, GPIO8); // LED8 nháy khi gửi thành công
//         }
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }

// static void task_can_rx(void *args) {
//     (void)args;
//     uint8_t rx_data[8];
//     uint32_t id;
//     bool ext, rtr;
//     uint8_t fmi, len;
//     while (1) {
//         if ((CAN_RF0R(CAN1) & CAN_RF0R_FMP_MASK) != 0) {
//             can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &len, rx_data, NULL);
//             gpio_toggle(GPIOB, GPIO9); // LED9 nháy khi nhận thành công
//         }
//         vTaskDelay(pdMS_TO_TICKS(100)); // Delay ngắn để giảm tải CPU
//     }
// }

// static void task_led_loop(void *args) {
//     (void)args;
//     while (1) {
//         gpio_toggle(GPIOC, GPIO13); // LED13 nháy báo trạng thái
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// int main(void) {
//     clock_setup();
//     gpio_setup();
//     can_setup();

//     // Tạo task FreeRTOS
//     xTaskCreate(task_can_tx, "CAN_TX", 128, NULL, 2, NULL);
//     xTaskCreate(task_can_rx, "CAN_RX", 128, NULL, 2, NULL);
//     xTaskCreate(task_led_loop, "LED_LOOP", 64, NULL, 1, NULL);

//     vTaskStartScheduler();

//     while (1); // Không bao giờ tới đây
// }

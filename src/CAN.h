/*
    Zerolife
    Hướng dẫn sử dụng:
    Nhận:
        uint64_t rx_data = can_recv(0x146);
        uint8_t data[8];
        for (int i = 0; i < 8; i++) 
        {
            data[i] = (rx_data >> (i * 8)) & 0xFF;
        }
    Gửi:
        uint8_t tx_data[8] = {'H', 'E', 'L', 'L', 'O', '!', '!', '\0'};
        can_send(0x146, tx_data);
 */

#ifndef CAN_H
#define CAN_H

#include "COMMON.h"

void can_setup()
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_CAN1);
    AFIO_MAPR |= AFIO_MAPR_CAN1_REMAP_PORTB;
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_CAN1_PB_TX); // PB9
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_CAN1_PB_RX); // PB8
    // gpio_set(GPIOB, GPIO8);
    can_reset(CAN1);
    can_init(CAN1,
            true,   // ttcm
            false,  // abom
            false,  // awum
            false,  // nart (bật retransmit)
            false,  // rflm
            false,  // txfp
            CAN_BTR_SJW_1TQ,
            CAN_BTR_TS1_13TQ,
            CAN_BTR_TS2_2TQ,
            212,    // ~10 kbps
            false,  // loopback
            false); // silent
    can_filter_init(0, true, false, 0, 0, 0, true);
}

int can_send(uint32_t can_id, uint8_t *data) 
{
    int result = can_transmit(CAN1, can_id, false, false, 8, data);
    if (result == -1) {
        CAN_TSR(CAN1) |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
    }
    return result;
}

uint64_t can_recv(uint32_t can_id) 
{
    uint8_t rx_data[8] = {0};
    uint32_t rx_id;
    bool ext, rtr;
    uint8_t fmi, dlc;

    uint32_t fmp0 = (CAN_RF0R(CAN1) & CAN_RF0R_FMP0_MASK);
    if (fmp0 > 0) {
        can_receive(CAN1, 0, true, &rx_id, &ext, &rtr, &fmi, &dlc, rx_data, NULL);
        if (rx_id == can_id && !ext && !rtr && dlc == 8) {
            uint64_t result = 0;
            for (int i = 0; i < 8; i++) {
                result |= ((uint64_t)rx_data[i]) << (i * 8);
            }
            return result;
        }
    }
    return -1;
}


#endif
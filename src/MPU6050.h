#ifndef MPU6050_H
#define MPU6050_H

#include "COMMON.h"
#include "TIME.h"

#define MPU6050_ADDR         0x68
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B

void mpu6050_init()
{
    i2c_send_start(I2C1);
    while (!(I2C_SR1(I2C1) & I2C_SR1_SB));

    i2c_send_7bit_address(I2C1, MPU6050_ADDR << 1, I2C_WRITE);
    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
    (void)I2C_SR2(I2C1); // clear ADDR

    i2c_send_data(I2C1, MPU6050_PWR_MGMT_1);
    while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));

    i2c_send_data(I2C1, 0x00);
    while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));

    i2c_send_stop(I2C1);
    delay_ms(100); // chờ sensor ổn định
}

void mpu6050_read(int16_t* ax, int16_t* ay, int16_t* az)
{
    uint8_t data[6];

    // Set register address
    i2c_send_start(I2C1);
    while (!(I2C_SR1(I2C1) & I2C_SR1_SB));

    i2c_send_7bit_address(I2C1, MPU6050_ADDR << 1, I2C_WRITE);
    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
    (void)I2C_SR2(I2C1);

    i2c_send_data(I2C1, MPU6050_ACCEL_XOUT_H);
    while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));

    // Repeated start
    i2c_send_start(I2C1);
    while (!(I2C_SR1(I2C1) & I2C_SR1_SB));

    i2c_send_7bit_address(I2C1, MPU6050_ADDR << 1, I2C_READ);
    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
    (void)I2C_SR2(I2C1);

    for (int i = 0; i < 6; i++) {
        while (!(I2C_SR1(I2C1) & I2C_SR1_RxNE));
        data[i] = i2c_get_data(I2C1);
    }

    i2c_send_stop(I2C1);

    *ax = (int16_t)((data[0] << 8) | data[1]);
    *ay = (int16_t)((data[2] << 8) | data[3]);
    *az = (int16_t)((data[4] << 8) | data[5]);
}

#endif

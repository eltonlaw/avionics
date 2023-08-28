#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "error.h"
#include "i2c.h"
#include "mpu6050.h"
#include "log.h"

void calibrate() {
    log_info("Starting calibration");
    // FIXME: gyroscope self test
}

bool init() {
    log_init();
    DDRB |= ((1 << PINB0) | (1 << PINB1));

    // init bus at 10 kHz
    i2c_init(10000UL);
    return E_OK;
}

int main() {
    avcerr_t err;
    mpu6050_reg_t mpu6050_reg;

    err = init();
    if (err == E_OK) {
        log_info("Finished initialization!");
    } else {
        panic("Failed to initialize: %d", err);
    }

    while (1) {
        // Toggle PB0/PB1
        PORTB ^= ((1 << PINB0) | (1 << PINB1));
        log_info("Flipped pins PORTB");

        // send start condition
        i2c_start();
        // send 7 bit i2c address of mpu6050 + write bit on bus
        // wait for ack from mpu6050
        i2c_write(MPU6050_I2C_SLA_W);
        // put register address on the bus
        // wait for ack from mpu6050
        i2c_write(MPU6050_ACCEL_XOUT_H);
        // send start condition
        i2c_start();
        // send 7 bit i2c address of mpu6050 + read bit on bus
        i2c_write(MPU6050_I2C_SLA_R);
        // wait for ack from mpu6050 otherwise send another start + address + read bit
        // wait for data from mpu6050
        mpu6050_reg.accel_x_h = i2c_read_ack();
        mpu6050_reg.accel_x_l = i2c_read_ack();
        mpu6050_reg.accel_y_h = i2c_read_ack();
        mpu6050_reg.accel_y_l = i2c_read_ack();
        mpu6050_reg.accel_z_h = i2c_read_ack();
        mpu6050_reg.accel_z_l = i2c_read_ack();
        mpu6050_reg.temp_h = i2c_read_ack();
        mpu6050_reg.temp_l = i2c_read_ack();
        mpu6050_reg.gyro_x_h = i2c_read_ack();
        mpu6050_reg.gyro_x_l = i2c_read_ack();
        mpu6050_reg.gyro_y_h = i2c_read_ack();
        mpu6050_reg.gyro_y_l = i2c_read_ack();
        mpu6050_reg.gyro_z_h = i2c_read_ack();
        mpu6050_reg.gyro_z_l = i2c_read_nack();
        log_info(
            "MPU6050 - Ax: %u,%u | Ay: %u,%u | Az: %u,%u | " "T: %u,%u | Gx: %u,%u | Gy: %u,%u | Gz: %u,%u",
            mpu6050_reg.accel_x_h, mpu6050_reg.accel_x_l,
            mpu6050_reg.accel_y_h, mpu6050_reg.accel_y_l,
            mpu6050_reg.accel_z_h, mpu6050_reg.accel_z_l, mpu6050_reg.temp_h,
            mpu6050_reg.temp_l, mpu6050_reg.gyro_x_h, mpu6050_reg.gyro_x_l,
            mpu6050_reg.gyro_y_h, mpu6050_reg.gyro_y_l, mpu6050_reg.gyro_z_h,
            mpu6050_reg.gyro_z_l
        );
        // Send stop condition
        i2c_stop();
        _delay_ms(1000);
    }
    return 0;
}

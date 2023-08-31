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

    while (true) {
        // Toggle PB0/PB1
        PORTB ^= ((1 << PINB0) | (1 << PINB1));

        mpu6050_read(&mpu6050_reg);
        log_info(
            "MPU6050 - Ax: %03u,%03u | Ay: %03u,%03u | Az: %03u,%03u | " "T: %03u,%03u | Gx: %03u,%03u | Gy: %03u,%03u | Gz: %03u,%03u",
            mpu6050_reg.accel_x_h, mpu6050_reg.accel_x_l,
            mpu6050_reg.accel_y_h, mpu6050_reg.accel_y_l,
            mpu6050_reg.accel_z_h, mpu6050_reg.accel_z_l, mpu6050_reg.temp_h,
            mpu6050_reg.temp_l, mpu6050_reg.gyro_x_h, mpu6050_reg.gyro_x_l,
            mpu6050_reg.gyro_y_h, mpu6050_reg.gyro_y_l, mpu6050_reg.gyro_z_h,
            mpu6050_reg.gyro_z_l
        );

        _delay_ms(1000);
    }
    return 0;
}

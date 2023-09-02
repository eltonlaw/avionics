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

    mpu6050_init(MPU6050_GYRO_RANGE_250, MPU6050_ACCEL_RANGE_2);
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
            "MPU6050 - Ax: %05d | Ay: %05d | Az: %05d | T: %05d | Gx: %05d | Gy: %05d | Gz: %05d",
            mpu6050_reg.accel_x,
            mpu6050_reg.accel_y,
            mpu6050_reg.accel_z,
            mpu6050_reg.temp,
            mpu6050_reg.gyro_x,
            mpu6050_reg.gyro_y,
            mpu6050_reg.gyro_z
        );
    }
    return 0;
}

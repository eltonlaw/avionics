#include "mpu6050.h"

const uint16_t i2c_timeout = 100;
const double Accel_Z_corrector = 14418.0;

uint8_t hello() {
    return 65;
}

error_t mpu6050_init(I2C_HandleTypeDef i2cx) {
    return E_OK;
}

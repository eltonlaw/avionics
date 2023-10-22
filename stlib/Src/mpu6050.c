#include "mpu6050.h"

const uint16_t i2c_timeout = 100;
const double Accel_Z_corrector = 14418.0;

error_t mpu6050_init(I2C_HandleTypeDef* i2cx) {
    uint8_t buf;
    HAL_StatusTypeDef status;

    /* FIXME: hardcode ranges for now to most sensitive */
    mpu6050_gyro_range_t gyro_range = MPU6050_GYRO_RANGE_250;
    mpu6050_accel_range_t accel_range = MPU6050_ACCEL_RANGE_2;

    HAL_I2C_Mem_Read(i2cx, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &buf, 1, HAL_MAX_DELAY);
    if (0x68 != buf) {
        return E_I2C_WRONG_DEVICE;
    }

    buf = MPU6050_PWR_MGMT_OFF;
    status = HAL_I2C_Mem_Write(i2cx, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &buf, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return (error_t) status;
    }

    // Set gyro config
    buf = (((1 << 2) - 1) & gyro_range) << 3;
    status = HAL_I2C_Mem_Write(i2cx, MPU6050_ADDR, MPU6050_GYRO_CONFIG_REG, 1, &buf, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return (error_t) status;
    }

    // Set accel config
    buf = (((1 << 2) - 1) & accel_range) << 3;
    status = HAL_I2C_Mem_Write(i2cx, MPU6050_ADDR, MPU6050_ACCEL_CONFIG_REG, 1, &buf, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return (error_t) status;
    }

    return E_OK;
}

error_t mpu6050_read(I2C_HandleTypeDef *i2cx, mpu6050_data_t *data) {
    uint8_t buf[14];
    HAL_StatusTypeDef status;

    mpu6050_config_t cfg = { 16384.0, 131.0 };

    status = HAL_I2C_Mem_Read(i2cx, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H_REG, 1, buf, 14, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return (error_t) status;
    }

    data->accel_x = (int16_t)(buf[0] << 8 | buf[1]) / cfg.accel_scaler;
    data->accel_y = (int16_t)(buf[2] << 8 | buf[3]) / cfg.accel_scaler;
    data->accel_z = (int16_t)(buf[4] << 8 | buf[5]) / 14418.0;
    data->temperature = (double)((int16_t)(buf[6] << 8 | buf[7]) / (double) 340.0 + (double) 36.53);
    data->gyro_x = (int16_t)(buf[8] << 8 | buf[9]) / cfg.gyro_scaler;
    data->gyro_y = (int16_t)(buf[10] << 8 | buf[11]) / cfg.gyro_scaler;
    data->gyro_z = (int16_t)(buf[12] << 8 | buf[13]) / cfg.gyro_scaler;

    return E_OK;
}

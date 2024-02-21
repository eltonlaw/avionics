#include "mpu6050.h"

const uint16_t i2c_timeout = 100;
const double Accel_Z_corrector = 14418.0;

/* Read data registers on MPU6050 with i2c bus */
error_t mpu6050_read(mpu6050_cfg_t* cfg, mpu6050_data_t *data) {
    uint8_t buf[14];
    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Read(cfg->i2cx, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H_REG, 1, buf, 14, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return (error_t) status;
    }

    data->accel_x = ((int16_t)(buf[0] << 8 | buf[1]) / cfg->accel_scaler) + cfg->offset.accel_x;
    data->accel_y = ((int16_t)(buf[2] << 8 | buf[3]) / cfg->accel_scaler) + cfg->offset.accel_y;
    data->accel_z = ((int16_t)(buf[4] << 8 | buf[5]) / 14418.0) + cfg->offset.accel_z;
    data->temperature = (double)((int16_t)(buf[6] << 8 | buf[7]) / (double) 340.0 + (double) 36.53);
    data->gyro_x = ((int16_t)(buf[8] << 8 | buf[9]) / cfg->gyro_scaler) + cfg->offset.gyro_x;
    data->gyro_y = ((int16_t)(buf[10] << 8 | buf[11]) / cfg->gyro_scaler) + cfg->offset.gyro_y;
    data->gyro_z = ((int16_t)(buf[12] << 8 | buf[13]) / cfg->gyro_scaler) + cfg->offset.gyro_z;

    return E_OK;
}

/* Calculate offsets for Ax,Ay,Az and Gx,Gy,Gz */
error_t mpu6050_calibrate(mpu6050_cfg_t* cfg) {
    // FIXME: detect if it's moving where possible
    // incrementally calculate average readings of offsets
    error_t status;
    mpu6050_data_t data;
    mpu6050_data_t offset;
    for (int i = 1; i < 700; i++) {
        if ((status = mpu6050_read(cfg, &data)) != E_OK) {
            return status;
        };
        offset.accel_x = offset.accel_x - (data.accel_x - offset.accel_x) / i;
        offset.accel_y = offset.accel_y - (data.accel_y - offset.accel_y) / i;
        // sensor should be positions such that it is facing away from earth
        offset.accel_z = offset.accel_z - (data.accel_z - offset.accel_z - 1) / i;
        offset.gyro_x = offset.gyro_x - (data.gyro_x - offset.gyro_x) / i;
        offset.gyro_y = offset.gyro_y - (data.gyro_y - offset.gyro_y) / i;
        offset.gyro_z = offset.gyro_z - (data.gyro_z - offset.gyro_z) / i;
    }
    cfg->offset = offset;
    return E_OK;
}

error_t mpu6050_init(mpu6050_cfg_t* cfg, I2C_HandleTypeDef* i2cx) {
    uint8_t buf;
    error_t status;

    cfg->i2cx = i2cx;
    cfg->offset = (mpu6050_data_t) {0, 0, 0, 0, 0, 0, 0};

    /* FIXME: hardcode ranges for now to most sensitive */
    mpu6050_gyro_range_t gyro_range = MPU6050_GYRO_RANGE_250;
    mpu6050_accel_range_t accel_range = MPU6050_ACCEL_RANGE_2;

    HAL_I2C_Mem_Read(i2cx, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &buf, 1, HAL_MAX_DELAY);
    if (0x68 != buf) {
        return E_I2C_WRONG_DEVICE;
    }

    buf = MPU6050_PWR_MGMT_OFF;
    status = (error_t) HAL_I2C_Mem_Write(i2cx, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &buf, 1, HAL_MAX_DELAY);
    if (status != E_HAL_OK) {
        return status;
    }

    // Set gyro config
    buf = (((1 << 2) - 1) & gyro_range) << 3;
    status = (error_t) HAL_I2C_Mem_Write(i2cx, MPU6050_ADDR, MPU6050_GYRO_CONFIG_REG, 1, &buf, 1, HAL_MAX_DELAY);
    if (status != E_HAL_OK) {
        return status;
    }
    switch(gyro_range) {
        case MPU6050_GYRO_RANGE_250:
            cfg->gyro_scaler = 131;
            break;
        case MPU6050_GYRO_RANGE_500:
            cfg->gyro_scaler = 65.5;
            break;
        case MPU6050_GYRO_RANGE_1000:
            cfg->gyro_scaler = 32.8;
            break;
        case MPU6050_GYRO_RANGE_2000:
            cfg->gyro_scaler = 16.4;
            break;
    }

    // Set accel config
    buf = (((1 << 2) - 1) & accel_range) << 3;
    status = HAL_I2C_Mem_Write(i2cx, MPU6050_ADDR, MPU6050_ACCEL_CONFIG_REG, 1, &buf, 1, HAL_MAX_DELAY);
    if (status != E_HAL_OK) {
        return (error_t) status;
    }
    switch(accel_range) {
        case MPU6050_ACCEL_RANGE_2:
            cfg->accel_scaler = 16384;
            break;
        case MPU6050_ACCEL_RANGE_4:
            cfg->accel_scaler = 8192;
            break;
        case MPU6050_ACCEL_RANGE_8:
            cfg->accel_scaler = 4096;
            break;
        case MPU6050_ACCEL_RANGE_16:
            cfg->accel_scaler = 2048;
            break;
    }

    if ((status = mpu6050_calibrate(cfg)) != E_OK) {
        return status;
    }

    return E_OK;
}

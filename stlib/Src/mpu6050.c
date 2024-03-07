#include "mpu6050.h"

/* Read data registers on MPU6050 with i2c bus */
error_t mpu6050_read(mpu6050_cfg_t* cfg, imu_data_t *data) {
    uint8_t buf[14];
    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Read(cfg->i2cx, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H_REG, 1, buf, 14, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return (error_t) status;
    }

    data->accel_x = ((int16_t)(buf[0] << 8 | buf[1]) / cfg->accel_scaler) - cfg->offset.accel_x;
    data->accel_y = ((int16_t)(buf[2] << 8 | buf[3]) / cfg->accel_scaler) - cfg->offset.accel_y;
    data->accel_z = ((int16_t)(buf[4] << 8 | buf[5]) / cfg->accel_scaler) - cfg->offset.accel_z;
    data->temperature = (double)((int16_t)(buf[6] << 8 | buf[7]) / (double) 340.0 + (double) 36.53);
    data->gyro_x = ((int16_t)(buf[8] << 8 | buf[9]) / cfg->gyro_scaler) - cfg->offset.gyro_x;
    data->gyro_y = ((int16_t)(buf[10] << 8 | buf[11]) / cfg->gyro_scaler) - cfg->offset.gyro_y;
    data->gyro_z = ((int16_t)(buf[12] << 8 | buf[13]) / cfg->gyro_scaler) - cfg->offset.gyro_z;

    return E_OK;
}

/* Calculate offsets for Ax,Ay,Az and Gx,Gy,Gz */
error_t mpu6050_calibrate(mpu6050_cfg_t* cfg) {
    // FIXME: detect if it's moving where possible
    // incrementally calculate average readings of offsets
    error_t status;
    uint8_t num_failed = 0;
    imu_data_t data;
    imu_data_t offset = {0, 0, 0, 0, 0, 0, 0};
    for (int i = 1; i < 700; i++) {
        if ((status = mpu6050_read(cfg, &data)) != E_OK) {
            num_failed++;
            if (num_failed > 70) {
                log_error("MPU6050 >10\% of 700 reads failed at step %d\n", i);
                return status;
            }
        };
        offset.accel_x = offset.accel_x + (data.accel_x - offset.accel_x) / i;
        offset.accel_y = offset.accel_y + (data.accel_y - offset.accel_y) / i;
        // sensor should be positions such that it is facing away from earth
        offset.accel_z = offset.accel_z + (data.accel_z - offset.accel_z - 1) / i;
        offset.gyro_x = offset.gyro_x + (data.gyro_x - offset.gyro_x) / i;
        offset.gyro_y = offset.gyro_y + (data.gyro_y - offset.gyro_y) / i;
        offset.gyro_z = offset.gyro_z + (data.gyro_z - offset.gyro_z) / i;
    }
    cfg->offset = offset;
    return E_OK;
}

error_t mpu6050_init(mpu6050_cfg_t* cfg) {
    uint8_t buf;
    error_t status;

    if (!cfg->whoami_checked) {
        HAL_I2C_Mem_Read(cfg->i2cx, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &buf, 1, HAL_MAX_DELAY);
        if (0x68 != buf) {
            log_error("MPU6050: WHO_AM_I register gave unexpected value: %x\n", buf);
            return E_I2C_WRONG_DEVICE;
        }
        cfg->whoami_checked = true;
    }

    if (!cfg->pwr_mgmt_off) {
        buf = MPU6050_PWR_MGMT_OFF;
        status = (error_t) HAL_I2C_Mem_Write(cfg->i2cx, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &buf, 1, HAL_MAX_DELAY);
        if (status != E_HAL_OK) {
            log_error("MPU6050: Failed to turn off sleep mode\n");
            return status;
        }
        cfg->pwr_mgmt_off = true;
    }

    // Set gyro config
    if (!cfg->gyro_range_set) {
        buf = (((1 << 2) - 1) & cfg->gyro_range) << 3;
        status = (error_t) HAL_I2C_Mem_Write(cfg->i2cx, MPU6050_ADDR, MPU6050_GYRO_CONFIG_REG, 1, &buf, 1, HAL_MAX_DELAY);
        if (status != E_HAL_OK) {
            return status;
        }
        switch(cfg->gyro_range) {
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
        cfg->gyro_range_set = true;
    }

    // Set accel config
    if (!cfg->accel_range_set) {
        buf = (((1 << 2) - 1) & cfg->accel_range) << 3;
        status = HAL_I2C_Mem_Write(cfg->i2cx, MPU6050_ADDR, MPU6050_ACCEL_CONFIG_REG, 1, &buf, 1, HAL_MAX_DELAY);
        if (status != E_HAL_OK) {
            return (error_t) status;
        }
        switch(cfg->accel_range) {
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
        cfg->accel_range_set = true;
    }

    if (!cfg->offsets_set) {
        cfg->offset = (imu_data_t) {0, 0, 0, 0, 0, 0, 0};
        if ((status = mpu6050_calibrate(cfg)) != E_OK) {
            return status;
        }
        cfg->offsets_set = true;
    }

    return E_OK;
}

error_t mpu6050_init_w_retry(mpu6050_cfg_t* cfg, uint8_t max_retries) {
    error_t err;
    uint8_t init_current_retry = 0;
    cfg->whoami_checked = false;
    cfg->pwr_mgmt_off = false;
    cfg->gyro_range_set = false;
    cfg->accel_range_set = false;
    cfg->offsets_set = false;
    while (1) {
        log_info("Attempt (%d) to initialize MPU6050\n", init_current_retry);
        err = mpu6050_init(cfg);
        init_current_retry++;
        // if successful or max retries reached, return
        if ((E_OK == err) || (init_current_retry >= max_retries)) {
            return err;
        }
    }
}

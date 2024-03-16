#include <stdint.h>
#include "control.h"
#include "icm20948.h"

error_t icm20948_select_bank(icm20948_cfg_t* cfg, unsigned char bank_id) {
    uint8_t buf[1];
    HAL_StatusTypeDef hal_status;
    // If already selected, short circuit
    if (cfg->bank_id == bank_id) {
        log_debug("ICM20948: Bank id %d already selected", bank_id);
        return E_OK;
    }
    // Only 4 banks: 0,1,2,3
    if ((bank_id > 3) || (bank_id < 0)) {
        panic("ICM20948: Invalid bank id provided: %d\n", bank_id);
        return E_INVALID_VALUE;
    }

    // in that register 5th and 6th bits determine bank, rest are reserved. Not
    // sure if used internally somehow to track state so going to read it in
    // then modify that value
    hal_status = HAL_I2C_Mem_Read(cfg->i2cx, ICM20948_ADDR_0,
            ICM20948_REG_BANK_SEL, 1, buf, 1, HAL_MAX_DELAY);
    if (hal_status != HAL_OK) {
        log_error("ICM20948: Failed to read existing bank select register");
        return (error_t) hal_status;
    }
    buf[0] &= ~(0x03 << 4);
    buf[0] |= (bank_id << 4);
    hal_status = HAL_I2C_Mem_Write(cfg->i2cx, ICM20948_ADDR_0,
            ICM20948_REG_BANK_SEL, 1, &buf[0], 1, HAL_MAX_DELAY);
    if (hal_status != HAL_OK) {
        log_error("ICM20948: Failed to write to bank select register\n");
        return (error_t) hal_status;
    }
    cfg->bank_id = bank_id;

    return E_OK;
}

error_t icm20948_read(icm20948_cfg_t* cfg, imu_data_t *data) {
    uint8_t buf[14];
    HAL_StatusTypeDef hal_status;
    error_t err;
    if (cfg->bank_id != 0) {
        log_warn("ICM20948: Bank id not set to 0 when trying to read from sensor registers");
        if (E_OK != (err = icm20948_select_bank(cfg, 0)))
            return err;
    }

    hal_status = HAL_I2C_Mem_Read(cfg->i2cx, ICM20948_ADDR_0, ICM20948_ACCEL_XOUT_H_REG, 1, buf, 14, HAL_MAX_DELAY);
    if (HAL_OK == hal_status) {
        log_error("ICM20948: Failed to read sensor value registers\n");
        return (error_t) hal_status;
    }

    data->accel_x = ((int16_t)(buf[0] << 8 | buf[1]) / cfg->accel_scaler) - cfg->offset.accel_x;
    data->accel_y = ((int16_t)(buf[2] << 8 | buf[3]) / cfg->accel_scaler) - cfg->offset.accel_y;
    data->accel_z = ((int16_t)(buf[4] << 8 | buf[5]) / cfg->accel_scaler) - cfg->offset.accel_z;
    data->gyro_x = ((int16_t)(buf[6] << 8 | buf[7]) / cfg->gyro_scaler) - cfg->offset.gyro_x;
    data->gyro_y = ((int16_t)(buf[8] << 8 | buf[9]) / cfg->gyro_scaler) - cfg->offset.gyro_y;
    data->gyro_z = ((int16_t)(buf[10] << 8 | buf[11]) / cfg->gyro_scaler) - cfg->offset.gyro_z;
    /* From Datasheet:
     * TEMP_degC = ((TEMP_OUT – RoomTemp_Offset)/Temp_Sensitivity) + 21degC
     * Typical Temp Sensitivity = 333.87 LSB / degC
     * Typical RoomTemp_Offset = 0 LSB at 21 degC */
    data->temperature = ((double)((int16_t)((buf[12] << 8) | buf[13])) / 333.87) + 21;

    return E_OK;
}

/* Calculate offsets for Ax,Ay,Az and Gx,Gy,Gz */
error_t icm20948_calibrate(icm20948_cfg_t* cfg) {
    // FIXME: detect if it's moving where possible
    // incrementally calculate average readings of offsets
    error_t err;
    uint8_t num_failed = 0;
    imu_data_t data;
    imu_data_t offset = {0, 0, 0, 0, 0, 0, 0};
    for (int i = 1; i < 700; i++) {
        if ((err = icm20948_read(cfg, &data)) != E_OK) {
            num_failed++;
            if (num_failed > 70) {
                log_error("ICM20948 >10\% of 700 reads failed at step %d\n", i);
                return err;
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

error_t icm20948_init(icm20948_cfg_t* cfg) {
    uint8_t buf;
    error_t err;
    HAL_StatusTypeDef hal_status;
    // Assign it to something that's not 0,1,2,3 that's in unsigned char range
    cfg->bank_id = 255;

    err = icm20948_select_bank(cfg, 0);
    if (err != E_OK) {
        return err;
    }
    if (!cfg->whoami_checked) {
        HAL_I2C_Mem_Read(cfg->i2cx, ICM20948_ADDR_0, ICM20948_WHO_AM_I_REG, 1, &buf, 1, HAL_MAX_DELAY);
        if (0xEA != buf) {
            log_error("ICM20948: WHO_AM_I register gave unexpected value: %x\n", buf);
            return E_I2C_WRONG_DEVICE;
        }
        cfg->whoami_checked = true;
    }

    if (!cfg->pwr_mgmt_configured) {
        // Default: 0x41, sleep bit is pin 6 in [7:0], clearing it wakes the chip up
        buf = 0x01;
        hal_status = HAL_I2C_Mem_Write(cfg->i2cx, ICM20948_ADDR_0, ICM20948_PWR_MGMT_1_REG, 1, &buf, 1, HAL_MAX_DELAY);
        if (hal_status != E_HAL_OK) {
            log_error("ICM20948: Failed to turn off sleep mode\n");
            return (error_t) hal_status;
        }
        cfg->pwr_mgmt_configured = true;
    }

    err = icm20948_select_bank(cfg, 2);
    // Set gyro config
    if (!cfg->gyro_range_set) {
        buf = (((1 << 2) - 1) & cfg->gyro_range) << 3;
        hal_status = HAL_I2C_Mem_Write(cfg->i2cx, ICM20948_ADDR_0,
                ICM20948_GYRO_CONFIG_1_REG, 1, &buf, 1, HAL_MAX_DELAY);
        if (hal_status != HAL_OK) {
            return (error_t) hal_status;
        }
        switch(cfg->gyro_range) {
            case ICM20948_GYRO_RANGE_250:
                cfg->gyro_scaler = 131;
                break;
            case ICM20948_GYRO_RANGE_500:
                cfg->gyro_scaler = 65.5;
                break;
            case ICM20948_GYRO_RANGE_1000:
                cfg->gyro_scaler = 32.8;
                break;
            case ICM20948_GYRO_RANGE_2000:
                cfg->gyro_scaler = 16.4;
                break;
        }
        cfg->gyro_range_set = true;
    }

    // // Set accel config
    // if (!cfg->accel_range_set) {
    //     buf = (((1 << 2) - 1) & cfg->accel_range) << 3;
    //     status = HAL_I2C_Mem_Write(cfg->i2cx, MPU6050_ADDR, MPU6050_ACCEL_CONFIG_REG, 1, &buf, 1, HAL_MAX_DELAY);
    //     if (status != E_HAL_OK) {
    //         return (error_t) status;
    //     }
    //     switch(cfg->accel_range) {
    //         case MPU6050_ACCEL_RANGE_2:
    //             cfg->accel_scaler = 16384;
    //             break;
    //         case MPU6050_ACCEL_RANGE_4:
    //             cfg->accel_scaler = 8192;
    //             break;
    //         case MPU6050_ACCEL_RANGE_8:
    //             cfg->accel_scaler = 4096;
    //             break;
    //         case MPU6050_ACCEL_RANGE_16:
    //             cfg->accel_scaler = 2048;
    //             break;
    //     }
    //     cfg->accel_range_set = true;
    // }

    // if (!cfg->offsets_set) {
    //     cfg->offset = (imu_data_t) {0, 0, 0, 0, 0, 0, 0};
    //     if ((status = icm20948_calibrate(cfg)) != E_OK) {
    //         return status;
    //     }
    //     cfg->offsets_set = true;
    // }

    // FIXME: add self test

    // ICM20948 read values
    err = icm20948_select_bank(cfg, 0);
    return E_OK;
}

error_t icm20948_init_w_retry(icm20948_cfg_t* cfg, uint8_t max_retries) {
    // error_t err;
    // uint8_t init_current_retry = 0;
    // cfg->whoami_checked = false;
    // cfg->pwr_mgmt_off = false;
    // cfg->gyro_range_set = false;
    // cfg->accel_range_set = false;
    // cfg->offsets_set = false;
    // while (1) {
    //     log_info("Attempt (%d) to initialize MPU6050\n", init_current_retry);
    //     err = icm20948_init(cfg);
    //     init_current_retry++;
    //     // if successful or max retries reached, return
    //     if ((E_OK == err) || (init_current_retry >= max_retries)) {
    //         return err;
    //     }
    // }
}

#include "icm20948.h"

const uint16_t i2c_timeout = 100;
const double Accel_Z_corrector = 14418.0;

error_t icm20948_selftest(icm20948_cfg_t* cfg) {
    // Get diff of sensor outputs with ST enabled and w/o ST enabled
    // Check diff within specified min/max limits.
    return E_OK;
}

error_t icm20948_sel_bank(icm20948_cfg_t* cfg, uint8_t bank) {
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(cfg->i2cx, ICM20948_ADDR0, ICM20948_REG_BANK_SEL_REG, 1, bank, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return (error_t) status;
    }
    return E_OK;
}

// Power on self test
error_t icm20948_post(icm20948_cfg_t* cfg) {
    return E_OK;
}

/* Read data registers on MPU6050 with i2c bus */
error_t icm20948_read(icm20948_cfg_t* cfg, icm20948_data_t *data) {
    uint8_t buf[14];
    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Read(cfg->i2cx, ICM20948_ADDR, ICM20948_ACCEL_XOUT_H_REG, 1, buf, 14, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return (error_t) status;
    }

    data->accel_x = ((int16_t)(buf[0] << 8 | buf[1]) / cfg->accel_scaler);
    data->accel_y = ((int16_t)(buf[2] << 8 | buf[3]) / cfg->accel_scaler);
    data->accel_z = ((int16_t)(buf[4] << 8 | buf[5]) / 14418.0);
    data->gyro_x = ((int16_t)(buf[6] << 8 | buf[7]) / cfg->gyro_scaler);
    data->gyro_y = ((int16_t)(buf[8] << 8 | buf[9]) / cfg->gyro_scaler);
    data->gyro_z = ((int16_t)(buf[10] << 8 | buf[11]) / cfg->gyro_scaler);
    data->temperature = (double)(((int16_t)((buf[12] << 8 | buf[13]) - cfg->temperature_offset) / cfg->temperature_scaler) + 21);

    return E_OK;
}

/* Calculate offsets for Ax,Ay,Az and Gx,Gy,Gz */
error_t icm20948_calibrate(icm20948_cfg_t* cfg) {
    return E_OK;
}

error_t icm20948_init(icm20948_cfg_t* cfg, I2C_HandleTypeDef* i2cx) {
    error_t err;
    HAL_StatusTypeDef status;
    uint8_t buf;

    status = HAL_I2C_Mem_Read(cfg->i2cx, ICM20948_ADDR, ICM20948_WHO_AM_I_REG, 1, buf, 1, HAL_MAX_DELAY);
    if (buf != 0xEA)
        return E_WRONG_WHOAMI;

    // Bank 0 mods
    if (err = icm20948_sel_bank(&cfg, ICM20948_BANK0) != E_OK)
        return err;

    // Turn on; Disable the SLEEP pin on bit 6 (NOTE: [7:0])
    // 0x41 is default value
    buf = 0x41 & ~(1 << 6);
    status = HAL_I2C_Mem_Write(cfg->i2cx, ICM20948_ADDR0, ICM20948_PWR_MGMT_1_REG, 1, buff, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return (error_t) status;
    }

    // Set sample rate divider to 0
    // Set gyro full scale range to 250dps
    // Set accel full scale range to 2g
    // Set DLPF to 17Hz
    // Set gyro DLPF to 17Hz
    // Set sample rate divider to 0
    // Set user bank 2
    // Set accel DLPF to 17Hz
    // Set user bank 0
    // Set power management 1 to wake up
    return err;
}

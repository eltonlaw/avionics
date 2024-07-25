/* bme280
 *
 * - pressure range (hPa): 300 to 1100
 *
 * A lot of code taken from https://github.com/boschsensortec/BME280_SensorAPI
 * */
#include "log.h"
#include "bme280.h"

/* Calibration data hardcoded into NVM during production */
error_t bme280_calibrate(bme280_cfg_t *cfg) {
    HAL_StatusTypeDef status;
    uint8_t buf[24];
    status = HAL_I2C_Mem_Read(cfg->i2cx, cfg->addr, BME280_REG_TEMP_PRESS_CALIB_DATA, 1, buf, 24, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        log_error("BME280 failed to read calibration data %d\n", status);
        return (error_t) status;
    }

    cfg->dig_t1 = (buf[1] << 8) | buf[0];
    cfg->dig_t2 = (int16_t)((buf[3] << 8) | buf[2]);
    cfg->dig_t3 = (int16_t)((buf[5] << 8) | buf[4]);
    cfg->dig_p1 = (buf[7] << 8) | buf[6];
    cfg->dig_p2 = (int16_t)((buf[9] << 8) | buf[8]);
    cfg->dig_p3 = (int16_t)((buf[11] << 8) | buf[10]);
    cfg->dig_p4 = (int16_t)((buf[13] << 8) | buf[12]);
    cfg->dig_p5 = (int16_t)((buf[15] << 8) | buf[14]);
    cfg->dig_p6 = (int16_t)((buf[17] << 8) | buf[16]);
    cfg->dig_p7 = (int16_t)((buf[19] << 8) | buf[18]);
    cfg->dig_p8 = (int16_t)((buf[21] << 8) | buf[20]);
    cfg->dig_p9 = (int16_t)((buf[23] << 8) | buf[22]);

    return E_OK;
}

error_t bme280_soft_reset(bme280_cfg_t *cfg) {
    HAL_StatusTypeDef status;
    uint8_t buf[1];
    buf[0] = BME280_SOFT_RESET_COMMAND;
    status = HAL_I2C_Mem_Write(cfg->i2cx, cfg->addr, BME280_REG_RESET, 1, buf, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        log_error("BME280 failed to write reset %d\n", status);
        return (error_t) status;
    }
    HAL_Delay(BME280_STARTUP_DELAY_MS);

    uint8_t retry_count = 10;
    while (retry_count--) {
        status = HAL_I2C_Mem_Read(cfg->i2cx, cfg->addr, BME280_REG_STATUS, 1, buf, 1, HAL_MAX_DELAY);
        if (status != HAL_OK) {
            log_error("BME280 failed to read status from 0xF3 %d\n", status);
            return (error_t) status;
        }
        if ((buf[0] & BME280_STATUS_IM_UPDATE) == 0) {
            break;
        }
    }
    if (retry_count == 0) {
        log_error("BME280 NVM data still being copied to register %d\n", status);
        return E_UNKNOWN;
    }

    return E_OK;
}

error_t bme280_read(bme280_cfg_t *cfg, bme280_data_t *data) {
    HAL_StatusTypeDef status;
    uint8_t buf[3];
    status = HAL_I2C_Mem_Read(cfg->i2cx, cfg->addr, BME280_REG_PRESS_MSB, 1, buf, 3, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        log_error("BME280 failed to read data %d\n", status);
        return (error_t) status;
    }
    /* read uncompensated pressure */
    data->pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);

    return E_OK;
}

error_t bme280_init(bme280_cfg_t *cfg) {
    error_t err;
    HAL_StatusTypeDef status;
    uint8_t buf[1];

    status = HAL_I2C_Mem_Read(cfg->i2cx, cfg->addr, BME280_REG_CHIP_ID, 1, buf, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        log_error("BME280 failed to read chip id %d\n", status);
        return (error_t) status;
    }
    if (buf[0] != BME280_CHIP_ID) {
        log_error("BME280 incorrect chip id E=%x A=%x\n", BME280_CHIP_ID, buf[0]);
        return E_I2C_WRONG_DEVICE;
    }
    err = bme280_soft_reset(cfg);
    if (err != E_OK) {
        return err;
    }
    err = bme280_calibrate(cfg);
    if (err != E_OK) {
        return err;
    }

    return E_OK;
}

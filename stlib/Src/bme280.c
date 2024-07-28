/* bme280
 *
 * - pressure range (hPa): 300 to 1100
 *
 * A lot of code taken from https://github.com/boschsensortec/BME280_SensorAPI
 * */
#include "log.h"
#include "bme280.h"

/* Calibration data hardcoded into NVM during production */
static error_t bme280_calibrate(bme280_cfg_t *cfg) {
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

static error_t bme280_soft_reset(bme280_cfg_t *cfg) {
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

/* Temperature compensation formula in bosch API */
static double bme280_comp_temp(bme280_cfg_t *cfg, int32_t t_adc) {
    double var1, var2, t_out;
    var1 = (((double) t_adc) / 16384.0 - ((double) cfg->dig_t1) / 1024.0) * ((double) cfg->dig_t2);
    var2 = (((double) t_adc) / 131072.0 - ((double) cfg->dig_t1) / 8192.0);
    var2 = var2 * var2 * ((double) cfg->dig_t3);
    cfg->t_fine = (int32_t) (var1 + var2);
    t_out = (var1 + var2) / 5120.0;
    if (t_out < BME280_TEMP_MIN) {
        t_out = BME280_TEMP_MIN;
    } else if (t_out > BME280_TEMP_MAX) {
        t_out = BME280_TEMP_MAX;
    }
    return t_out;
}

/* Pressure compensation formula in bosch API */
static double bme280_comp_press(bme280_cfg_t *cfg, uint32_t p_adc) {
    double var1, var2, p_out;
    var1 = ((double) cfg->t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double) cfg->dig_p6) / 32768.0;
    var2 = var2 + var1 * ((double) cfg->dig_p5) * 2.0;
    var2 = (var2 / 4.0) + (((double) cfg->dig_p4) * 65536.0);
    var1 = (((double) cfg->dig_p3) * var1 * var1 / 524288.0 + ((double) cfg->dig_p2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double) cfg->dig_p1);
    if (var1 == 0.0) {
        return 0; // avoid exception caused by division by zero
    }
    p_out = 1048576.0 - (double) p_adc;
    p_out = (p_out - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double) cfg->dig_p9) * p_out * p_out / 2147483648.0;
    var2 = p_out * ((double) cfg->dig_p8) / 32768.0;
    p_out = p_out + (var1 + var2 + ((double) cfg->dig_p7)) / 16.0;
    if (p_out < BME280_PRESS_MIN) {
        p_out = BME280_PRESS_MIN;
    } else if (p_out > BME280_PRESS_MAX) {
        p_out = BME280_PRESS_MAX;
    }
    return p_out;
}

static void bme280_comp(bme280_cfg_t *cfg, bme280_data_t *data){
    data->temperature = bme280_comp_temp(cfg, data->temperature);
    data->pressure = bme280_comp_press(cfg, data->pressure);
}

error_t bme280_read(bme280_cfg_t *cfg, bme280_data_t *data) {
    HAL_StatusTypeDef status;
    uint8_t buf[6];
    status = HAL_I2C_Mem_Read(cfg->i2cx, cfg->addr, BME280_REG_PRESS_MSB, 1, buf, 6, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        log_error("BME280 failed to read data %d\n", status);
        return (error_t) status;
    }
    /* read uncompensated pressure and temperature */
    data->pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
    data->temperature = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4);
    bme280_comp(cfg, data);

    return E_OK;
}

error_t bme280_change_mode(uint8_t mode) {
    HAL_StatusTypeDef status;
    uint8_t buf[1];
    status = HAL_I2C_Mem_Read(cfg->i2cx, cfg->addr, BME280_REG_PWR_CTRL, 1, buf, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        log_error("BME280 failed to read power control %d\n", status);
        return (error_t) status;
    }
    buf[0] = (buf[0] & 0xFC) | mode;
    status = HAL_I2C_Mem_Write(cfg->i2cx, cfg->addr, BME280_REG_PWR_CTRL, 1, buf, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        log_error("BME280 failed to write power control %d\n", status);
        return (error_t) status;
    }
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
    if ((bme280_soft_reset(cfg)) != E_OK) {
        return err;
    }
    if ((bme280_calibrate(cfg)) != E_OK) {
        return err;
    }
    /* By default, after powering up the sensor is in sleep mode. Change to normal mode
     * for continuous measurements */
    if ((err = bme280_change_mode(BME280_PWR_MODE_NORMAL)) != E_OK) {
        return err;
    }

    return E_OK;
}

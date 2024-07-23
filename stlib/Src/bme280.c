/* bme280
 *
 * - pressure range (hPa): 300 to 1100
 * */
#include "log.h"
#include "bme280.h"

error_t bme280_soft_reset(bme280_cfg_t *cfg) {
    HAL_StatusTypeDef status;
    uint8_t buf[1];
    buf[0] = BME280_SOFT_RESET_COMMAND;
    status = HAL_I2C_Mem_Write(cfg->i2cx, cfg->addr, BME280_RESET, 1, buf, 1, HAL_MAX_DELAY);
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

error_t bme280_init(bme280_cfg_t *cfg) {
    error_t err;
    HAL_StatusTypeDef status;
    uint8_t buf[1];

    status = HAL_I2C_Mem_Read(cfg->i2cx, cfg->addr, BME280_CHIP_ID, 1, buf, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        log_error("BME280 failed to read chip id %d\n", status);
        return (error_t) status;
    }
    if (buf[0] != BME280_CHIP_ID) {
        log_error("BME280 incorrect chip id %d != %d\n", BME280_CHIP_ID, buf[0]);
        return E_I2C_WRONG_DEVICE;
    }
    err = bme280_soft_reset(cfg);
    if (err != E_OK) {
        return err;
    }

    return E_OK;
}

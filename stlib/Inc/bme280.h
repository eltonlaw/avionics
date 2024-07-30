#include "stm32g0xx.h"
#include "error.h"
/* 0x76 when SDO pulled low and 0x77 when SDO pulled high */
#define BME280_ADDR_0 (0x76 << 1)
#define BME280_ADDR_1 (0x77 << 1)

/* Time to first after both V_dd > 1.58V and V_ddio > 0.65V is 2 ms */
#define BME280_STARTUP_DELAY_MS 2

#define BME280_REG_TEMP_CALIB_DATA 0x88

#define BME280_REG_CHIP_ID 0xD0
#define BME280_CHIP_ID 0x60

#define BME280_REG_RESET 0xE0
/* Special value that when written to reset register will using complete power-on-reset procedure */
#define BME280_SOFT_RESET_COMMAND 0xB6

/* bit 0 is im_update, from datasheet "set to 1 when the NV data being copied
 * to image registers and back to '0' when the copying is done"*/
#define BME280_REG_STATUS 0xF3
#define BME280_STATUS_IM_UPDATE 0x01

/* Change Operation mode by changing pins [1:0]
 * Sleep=00
 * Normal=11
 * ForcedMeasurement=01 */
#define BME280_REG_PWR_CTRL 0xF4
#define BME280_PWR_MODE_SLEEP 0x00
#define BME280_PWR_MODE_FORCED 0x01
#define BME280_PWR_MODE_NORMAL 0x03

/* 0xF7 to 0xF9 is pressure */
#define BME280_REG_PRESS_MSB 0xF7
/* 0xFA to 0xFC is temperature */
#define BME280_REG_TEMP_MSB 0xFA

#define BME280_TEMP_MIN -40
#define BME280_TEMP_MAX 85

#define BME280_PRESS_MIN 30000.0
#define BME280_PRESS_MAX 110000.0

typedef struct {
    I2C_HandleTypeDef *i2cx;
    uint8_t addr;
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;
    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
    int32_t t_fine;
} bme280_cfg_t;

typedef struct {
    uint32_t pressure;
    int32_t temperature;
    uint32_t humidity;
} bme280_data_t;

error_t bme280_init(bme280_cfg_t *cfg);
error_t bme280_read(bme280_cfg_t *cfg, bme280_data_t *data);

#include "stm32g0xx.h"
#include "error.h"
/* 0x76 when SDO pulled low and 0x77 when SDO pulled high */
#define BME280_ADDR_0 0x76
#define BME280_ADDR_1 0x77

#define BME280_CHIP_ID 0x60

#define BME280_REG_RESET 0xE0
/* Special value that when written to reset register will using complete power-on-reset procedure */
#define BME280_SOFT_RESET_COMMAND 0xB6

/* bit 0 is im_update, from datasheet "set to 1 when the NV data being copied
 * to image registers and back to '0' when the copying is done"*/
#define BME280_REG_STATUS 0xF3
#define BME280_STATUS_IM_UPDATE 0x01

/* Time to first after both V_dd > 1.58V and V_ddio > 0.65V is 2 ms */
#define BME280_STARTUP_DELAY_MS 2

#define BME280_REG_PRESS_MSB 0xF7

typedef struct {
    I2C_HandleTypeDef *i2cx;
    uint8_t addr;
} bme280_cfg_t;

typedef struct {
    uint32_t pressure;
} bme280_data_t;

error_t bme280_init(bme280_cfg_t *cfg);

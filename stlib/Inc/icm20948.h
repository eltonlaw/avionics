#pragma once

#include <stdbool.h>
#include "stm32g0xx_hal.h"
#include "control.h"
#include "error.h"

// Depending on if AD0 is logic low/high (in that order)
#define ICM20948_ADDR_0 0b1101000
#define ICM20948_ADDR_1 0b1101001

#define ICM20948_REG_BANK_SEL 0x7F
// user bank register 0
#define ICM20948_WHO_AM_I_REG 0x00
#define ICM20948_PWR_MGMT_1_REG 0x06
#define ICM20948_ACCEL_XOUT_H_REG 0x2D

// User bank register 2
#define ICM20948_ACCEL_CONFIG_REG 0x14
#define ICM20948_ACCEL_CONFIG_2_REG 0x15
#define ICM20948_GYRO_CONFIG_1_REG 0x01
#define ICM20948_GYRO_CONFIG_2_REG 0x02

// Enables +-250/500/1000/2000 degrees per second
typedef enum {
    ICM20948_GYRO_RANGE_250 = 0,
    ICM20948_GYRO_RANGE_500,
    ICM20948_GYRO_RANGE_1000,
    ICM20948_GYRO_RANGE_2000,
} icm20948_gyro_range_t;

typedef enum {
    ICM20948_ACCEL_RANGE_2 = 0,
    ICM20948_ACCEL_RANGE_4,
    ICM20948_ACCEL_RANGE_8,
    ICM20948_ACCEL_RANGE_16,
} icm20948_accel_range_t;

typedef struct {
    double accel_scaler;
    double gyro_scaler;
    icm20948_accel_range_t accel_range;
    icm20948_gyro_range_t gyro_range;
    I2C_HandleTypeDef* i2cx;
    TIM_HandleTypeDef* timx;
    imu_data_t offset;
    bool whoami_checked;
    bool pwr_mgmt_configured;
    bool gyro_range_set;
    bool accel_range_set;
    bool offsets_set;
    unsigned char bank_id;
} icm20948_cfg_t;

error_t icm20948_init(icm20948_cfg_t* cfg);
error_t icm20948_read(icm20948_cfg_t* cfg, imu_data_t *data);

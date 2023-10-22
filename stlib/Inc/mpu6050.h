#pragma once

#include <math.h>
#include <stdint.h>
#include "stm32g0xx.h"
#include "error.h"

#define MPU6050_ADDR (0x68 << 1)

#define MPU6050_GYRO_CONFIG_REG 0x1B
#define MPU6050_ACCEL_CONFIG_REG 0x1C
#define MPU6050_ACCEL_XOUT_H_REG 0x3B
#define MPU6050_PWR_MGMT_1_REG 0x6B
#define MPU6050_WHO_AM_I_REG 0x75

#define MPU6050_PWR_MGMT_OFF 0

typedef enum {
    MPU6050_GYRO_RANGE_250 = 0,
    MPU6050_GYRO_RANGE_500,
    MPU6050_GYRO_RANGE_1000,
    MPU6050_GYRO_RANGE_2000,
} mpu6050_gyro_range_t;

typedef enum {
    MPU6050_ACCEL_RANGE_2 = 0,
    MPU6050_ACCEL_RANGE_4,
    MPU6050_ACCEL_RANGE_8,
    MPU6050_ACCEL_RANGE_16,
} mpu6050_accel_range_t;

typedef struct {
    double accel_x;
    double accel_y;
    double accel_z;
    double temperature;
    double gyro_x;
    double gyro_y;
    double gyro_z;
} mpu6050_data_t;

typedef struct {
    double accel_scaler;
    double gyro_scaler;
    I2C_HandleTypeDef* i2cx;
} mpu6050_cfg_t;

error_t mpu6050_init(mpu6050_cfg_t* cfg, I2C_HandleTypeDef* i2cx);
error_t mpu6050_read(mpu6050_cfg_t* cfg, mpu6050_data_t *data);

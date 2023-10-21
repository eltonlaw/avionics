#include <math.h>
#include <stdint.h>
#include "error.h"

#define MPU6050_WHO_AM_I_REG 0x75
#define MPU6050_PWR_MGMT_1_REG 0x6B
#define MPU6050_ACCEL_CONFIG_REG 0x1C
#define MPU6050_ACCEL_XOUT_H_REG 0x3B
#define MPU6050_TEMP_OUT_H_REG 0x41
#define MPU6050_GYRO_CONFIG_REG 0x1B
#define MPU6050_GYRO_XOUT_H_REG 0x43
#define MPU6050_ADDR 0xD0

uint8_t hello();
error_t mpu6050_init(I2C_HandleTypeDef);

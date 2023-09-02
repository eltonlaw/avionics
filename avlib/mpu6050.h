#pragma once

#include <util/twi.h>

#define MPU6050_REG_ELF_TEST_X 0x0D
#define MPU6050_REG_ELF_TEST_Y 0x0E
#define MPU6050_REG_ELF_TEST_Z 0x0F
#define MPU6050_REG_ELF_TEST_A 0x10

#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_ACCEL_XOUT_L 0x3C
#define MPU6050_REG_ACCEL_YOUT_H 0x3D
#define MPU6050_REG_ACCEL_YOUT_L 0x3E
#define MPU6050_REG_ACCEL_ZOUT_H 0x3f
#define MPU6050_REG_ACCEL_ZOUT_L 0x40
#define MPU6050_REG_TEMP_OUT_H   0x41
#define MPU6050_REG_TEMP_OUT_L   0x42
#define MPU6050_REG_GYRO_XOUT_H  0x43
#define MPU6050_REG_GYRO_XOUT_L  0x44
#define MPU6050_REG_GYRO_YOUT_H  0x45
#define MPU6050_REG_GYRO_YOUT_L  0x46
#define MPU6050_REG_GYRO_ZOUT_H  0x47
#define MPU6050_REG_GYRO_ZOUT_L  0x48

#define MPU6050_REG_PWR_MGMT 0x6B
#define MPU6050_PWR_MGMT_OFF 0

#define MPU6050_REG_GYRO_CONFIG  0x1B
#define MPU6050_GYRO_RANGE_250 0
#define MPU6050_GYRO_RANGE_500 1
#define MPU6050_GYRO_RANGE_1000 2
#define MPU6050_GYRO_RANGE_2000 3

#define MPU6050_REG_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_RANGE_2 0
#define MPU6050_ACCEL_RANGE_4 1
#define MPU6050_ACCEL_RANGE_8 2
#define MPU6050_ACCEL_RANGE_16 3

#define MPU6050_I2C_SLA 0x68
// 7bit address + R/W bit
#define MPU6050_I2C_SLA_W (MPU6050_I2C_SLA << 1 | TW_WRITE)
#define MPU6050_I2C_SLA_R (MPU6050_I2C_SLA << 1 | TW_READ)

typedef struct {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t temp;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
} mpu6050_reg_t;

void mpu6050_init(uint8_t gyro_range, uint8_t accel_range);
void mpu6050_read(mpu6050_reg_t* reg);

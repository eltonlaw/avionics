#pragma once

#define MPU6050_SELF_TEST_X 0x0D
#define MPU6050_SELF_TEST_Y 0x0E
#define MPU6050_SELF_TEST_Z 0x0F
#define MPU6050_SELF_TEST_A 0x10

#define MPU6050_CONFIG  0x1A
#define MPU6050_GYRO_CONFIG  0x1B
#define MPU6050_ACCEL_CONFIG 0x1C

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3f
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H   0x41
#define MPU6050_TEMP_OUT_L   0x42
#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_GYRO_XOUT_L  0x44
#define MPU6050_GYRO_YOUT_H  0x45
#define MPU6050_GYRO_YOUT_L  0x46
#define MPU6050_GYRO_ZOUT_H  0x47
#define MPU6050_GYRO_ZOUT_L  0x48

#define MPU6050_I2C_SLA 0x68
// 7bit address + R/W bit
#define MPU6050_I2C_SLA_W (MPU6050_I2C_SLA << 1 | I2C_WRITE)
#define MPU6050_I2C_SLA_R (MPU6050_I2C_SLA << 1 | I2C_READ)

typedef struct {
    uint8_t accel_x_h;
    uint8_t accel_x_l;
    uint8_t accel_y_h;
    uint8_t accel_y_l;
    uint8_t accel_z_h;
    uint8_t accel_z_l;
    uint8_t temp_h;
    uint8_t temp_l;
    uint8_t gyro_x_h;
    uint8_t gyro_x_l;
    uint8_t gyro_y_h;
    uint8_t gyro_y_l;
    uint8_t gyro_z_h;
    uint8_t gyro_z_l;
} mpu6050_reg_t;

void mpu6050_read(mpu6050_reg_t* reg);

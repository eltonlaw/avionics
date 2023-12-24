/*
 * icm20948 can do I2C at 400kHz or SPI at 7MHz
 * SDA and SCL lines need pull-up resistors to VDD
 *
 * GYRO_FS_SEL=0 +=250
 * GYRO_FS_SEL=1 +=500
 * GYRO_FS_SEL=2 +=1000
 * GYRO_FS_SEL=3 +=2000
 */
#include "stm32g0xx.h"
#include "error.h"

// 7:6 and 3:0 unused. Set 5:4 to 0 for B0, 1 for B1, 2 for B2, 3 for B3
#define ICM20948_BANK0 0b00000000
#define ICM20948_BANK1 0b00010000
#define ICM20948_BANK2 0b00100000
#define ICM20948_BANK3 0b00110000

// Always acts as slave device so LSB is not R/W but an extra address
#define ICM20948_I2C_ADDR0 0b1101000
#define ICM20948_I2C_ADDR1 0b1101001

// REG_BANK_SEL defined on every register as 0x7F
#define ICM20948_REG_BANK_SEL_REG 0x7F // Selects a user bank

/* User bank 0 registers */
#define ICM20948_WHO_AM_I_REG 0x00
// Next 10 registers are accel (3D), gyro (3D) and temp output H/L registers
#define ICM20948_ACCEL_XOUT_H_REG 0x2D
// Default: 0x41. Sleep enabled by default. Set bit 7 to reset
#define ICM20948_PWR_MGMT_1_REG 0x06
// Default: 0x00. Set [5:3] to 111 to disable all accels, set [2:0] to 111 to disable gyros
#define ICM20948_PWR_MGMT_2_REG 0x07

/* User bank 1 registers */
#define ICM20948_XA_OFFS_H_REG 0x14
#define ICM20948_XA_OFFS_L_REG 0x15
#define ICM20948_YA_OFFS_H_REG 0x17
#define ICM20948_YA_OFFS_L_REG 0x18
#define ICM20948_ZA_OFFS_H_REG 0x1A
#define ICM20948_ZA_OFFS_L_REG 0x1B

/* User bank 2 registers */
#define ICM20948_GYRO_SMPLRT_DIV_REG 0x00
#define ICM20948_GYRO_CONFIG_1_REG 0x01
#define ICM20948_GYRO_CONFIG_2_REG 0x01

typedef struct {
    double accel_x;
    double accel_y;
    double accel_z;
    double temperature;
    double gyro_x;
    double gyro_y;
    double gyro_z;
} icm20948_data_t;

typedef struct {
    double accel_scaler;
    double gyro_scaler;
    double temperature_offset;
    double temperature_scaler;
    I2C_HandleTypeDef* i2cx;
} icm20948_cfg_t;

error_t icm20948_selftest(icm20948_cfg_t* cfg);
error_t icm20948_read(icm20948_cfg_t* cfg, icm20948_data_t *data);
error_t icm20948_calibrate(icm20948_cfg_t* cfg);
error_t icm20948_init(icm20948_cfg_t* cfg, I2C_HandleTypeDef* i2cx);

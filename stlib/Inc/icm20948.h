/*
 * icm20948 can do I2C at 400kHz or SPI at 7MHz
 * SDA and SCL lines need pull-up resistors to VDD
 */

// 7:6 and 3:0 unused. Set 5:4 to 0 for B0, 1 for B1, 2 for B2, 3 for B3
#define ICM20948_BANK0 0b00000000
#define ICM20948_BANK1 0b00010000
#define ICM20948_BANK2 0b00100000
#define ICM20948_BANK3 0b00110000

// Always acts as slave device so LSB is not R/W but an extra address
#define ICM20948_I2C_ADDR0 0b1101000
#define ICM20948_I2C_ADDR1 0b1101001

// REG_BANK_SEL defined on every register as 0x7F
#define ICM20948_REG_BANK_SEL 0x7F // Selects a user bank

/* User bank 0 registers */
// Next 10 registers are accel (3D), gyro (3D) and temp output H/L registers
#define ICM20948_ACCEL_XOUT_H 0x2D

                                   //
void icm20948_init(void) {
    // Set user bank 0
}

void icm20948_init(void) {
    // Set user bank 0
    i2c_write(ICM20948_I2C_ADDR0, ICM20948_REG_BANK_SEL, ICM20948_BANK0);
    // Set sample rate divider to 0
    i2c_write(ICM20948_I2C_ADDR0, 0x00, 0x00);
    // Set gyro full scale range to 250dps
    i2c_write(ICM20948_I2C_ADDR0, 0x01, 0x00);
    // Set accel full scale range to 2g
    i2c_write(ICM20948_I2C_ADDR0, 0x02, 0x00);
    // Set DLPF to 17Hz
    i2c_write(ICM20948_I2C_ADDR0, 0x03, 0x01);
    // Set gyro DLPF to 17Hz
    i2c_write(ICM20948_I2C_ADDR0, 0x04, 0x01);
    // Set sample rate divider to 0
    i2c_write(ICM20948_I2C_ADDR0, 0x00, 0x00);
    // Set user bank 2
    i2c_write(ICM20948_I2C_ADDR0, ICM20948_REG_BANK_SEL, ICM20948_BANK2);
    // Set accel DLPF to 17Hz
    i2c_write(ICM20948_I2C_ADDR0, 0x0C, 0x01);
    // Set user bank 0
    i2c_write(ICM20948_I2C_ADDR0, ICM20948_REG_BANK_SEL, ICM20948_BANK0);
    // Set power management 1 to wake up
    i2c_write(ICM20948_I2C_ADDR0, 0x06, 0x01);
}
void icm20948_init(void) {
    // Set user bank 0
    i2c_write(ICM20948_I2C_ADDR0, ICM20948_REG_BANK_SEL, ICM20948_BANK0);
    // Set sample rate divider to 0
    i2c_write(ICM20948_I2C_ADDR0, 0x00, 0x00);
    // Set gyro full scale range to 250dps
    i2c_write(ICM20948_I2C_ADDR0, 0x01, 0x00);
    // Set accel full scale range to 2g
    i2c_write(ICM20948_I2C_ADDR0, 0x02, 0x00);
    // Set DLPF to 17Hz
    i2c_write(ICM20948_I2C_ADDR0, 0x03, 0x01);
    // Set gyro DLPF to 17Hz
    i2c_write(ICM20948_I2C_ADDR0, 0x04, 0x01);
    // Set sample rate divider to 0
    i2c_write(ICM20948_I2C_ADDR0, 0x00, 0x00);
    // Set user bank 2
    i2c_write(ICM20948_I2C_ADDR0, ICM20948_REG_BANK_SEL, ICM20948_BANK2);
    // Set accel DLPF to 17Hz
    i2c_write(ICM20948_I2C_ADDR0, 0x0C, 0x01);
    // Set user bank 0
    i2c_write(ICM20948_I2C_ADDR0, ICM20948_REG_BANK_SEL, ICM20948_BANK0);
    // Set power management 1 to wake up
    i2c_write(ICM20948_I2C_ADDR0, 0x06, 0x01);
}

void icm20948_init(void) {
    // Set user bank 0
    i2c_write(ICM20948_I2C_ADDR0, ICM20948_REG_BANK_SEL, ICM20948_BANK0);
    // Set sample rate divider to 0
    i2c_write(ICM20948_I2C_ADDR0, 0x00, 0x00);
    // Set gyro full scale range to 250dps
    i2c_write(ICM20948_I2C_ADDR0, 0x01, 0x00);
    // Set accel full scale range to 2g
    i2c_write(ICM20948_I2C_ADDR0, 0x02, 0x00);
    // Set DLPF to 17Hz
    i2c_write(ICM20948_I2C_ADDR0, 0x03, 0x01);
    // Set gyro DLPF to 17Hz
    i2c_write(ICM20948_I2C_ADDR0, 0x04, 0x01);
    // Set sample rate divider to 0
    i2c_write(ICM20948_I2C_ADDR0, 0x00, 0x00);
    // Set user bank 2
    i2c_write(ICM20948_I2C_ADDR0, ICM20948_REG_BANK_SEL, ICM20948_BANK2);
    // Set accel DLPF to 17Hz
    i2c_write(ICM20948_I2C_ADDR0, 0x0C, 0x01);
    // Set user bank 0
    i2c_write(ICM20948_I2C_ADDR0, ICM20948_REG_BANK_SEL, ICM20948_BANK0);
    // Set power management 1 to wake up
    i2c_write(ICM20948_I2C_ADDR0, 0x06, 0x01);
}

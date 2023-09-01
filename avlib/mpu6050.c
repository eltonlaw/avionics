#include <stdint.h>
#include "i2c.h"
#include "mpu6050.h"
#include "log.h"

void mpu6050_init(uint8_t gyro_range, uint8_t accel_range) {
    i2c_start();

    /* Disable sleep mode, returns 0's in sensor registers otherwise */
    i2c_write_sla_w(MPU6050_I2C_SLA_W);
    i2c_write_data(MPU6050_REG_PWR_MGMT);
    i2c_write_data(MPU6050_PWR_MGMT_OFF);

    /* Read in gyro config and accel config, set range on specific bits */
    i2c_start();
    i2c_write_sla_w(MPU6050_I2C_SLA_W);
    i2c_write_data(MPU6050_REG_GYRO_CONFIG);
    i2c_start();
    i2c_write_sla_r(MPU6050_I2C_SLA_R);
    // On init these seem to be 0
    uint8_t gyro_config = i2c_read_ack();
    log_info("Read gyro config: %x", gyro_config);
    uint8_t accel_config = i2c_read_nack();
    log_info("Read accel config: %x", accel_config);
    gyro_config &= (((1 << 2) - 1) & gyro_range) << 3;
    accel_config &= (((1 << 2) - 1) & accel_range) << 3;
    i2c_start();
    i2c_write_sla_w(MPU6050_I2C_SLA_W);
    i2c_write_data(MPU6050_REG_GYRO_CONFIG);
    i2c_write_data(gyro_config);
    i2c_start();
    i2c_write_sla_w(MPU6050_I2C_SLA_W);
    i2c_write_data(MPU6050_REG_ACCEL_CONFIG);
    i2c_write_data(accel_config);

    i2c_stop();
}

void mpu6050_read(mpu6050_reg_t* reg) {
    i2c_start();
    // send 7 bit i2c address of mpu6050 + write bit on bus
    // wait for ack from mpu6050
    i2c_write_sla_w(MPU6050_I2C_SLA_W);
    // put register address on the bus
    // wait for ack from mpu6050
    i2c_write_data(MPU6050_REG_ACCEL_XOUT_H);
    // send start condition
    i2c_start();
    // send 7 bit i2c address of mpu6050 + read bit on bus
    i2c_write_sla_r(MPU6050_I2C_SLA_R);

    // burst read, keep sending acks, MPU6050 will
    // auto-inc address being read.
    reg->accel_x_h = i2c_read_ack();
    reg->accel_x_l = i2c_read_ack();
    reg->accel_y_h = i2c_read_ack();
    reg->accel_y_l = i2c_read_ack();
    reg->accel_z_h = i2c_read_ack();
    reg->accel_z_l = i2c_read_ack();
    reg->temp_h = i2c_read_ack();
    reg->temp_l = i2c_read_ack();
    reg->gyro_x_h = i2c_read_ack();
    reg->gyro_x_l = i2c_read_ack();
    reg->gyro_y_h = i2c_read_ack();
    reg->gyro_y_l = i2c_read_ack();
    reg->gyro_z_h = i2c_read_ack();
    reg->gyro_z_l = i2c_read_nack();
    // Send stop condition
    i2c_stop();
}

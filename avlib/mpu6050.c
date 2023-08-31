#include <stdint.h>
#include "i2c.h"
#include "mpu6050.h"
#include "log.h"

void mpu6050_read(mpu6050_reg_t* reg) {
    // send start condition
    i2c_start();
    // send 7 bit i2c address of mpu6050 + write bit on bus
    // wait for ack from mpu6050
    i2c_write_sla_w(MPU6050_I2C_SLA_W);
    // put register address on the bus
    // wait for ack from mpu6050
    i2c_write_data(MPU6050_ACCEL_XOUT_H);
    // send start condition
    i2c_start();
    // send 7 bit i2c address of mpu6050 + read bit on bus
    i2c_write(MPU6050_I2C_SLA_R);

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

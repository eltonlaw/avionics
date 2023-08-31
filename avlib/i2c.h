#pragma once
#include "error.h"

void i2c_init(unsigned long bus_speed);
void i2c_wait_for_complete(void);
avcerr_t i2c_start(void);
void i2c_stop(void);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
void i2c_write(uint8_t data);
void i2c_write_sla_w(uint8_t data);
void i2c_write_data(uint8_t data);
uint8_t i2c_status(void);

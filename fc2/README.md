# fc2

## fc2_board

- MPU6050 REGOUT (pin 10) needs 0.1uF regulator filter cap [^1]
- MPU6050 VDD (pin 13) needs 0.1uF bypass cap [^1]
- MPU6050 CPOUT (pin 20) needs 2.2nF charge pump cap [^1]
- MPU6050 VLOGIC (pin 8) needs 10nF bypass cap [^1]
- MPU6050 FSYNC (pin 11) connect to GND since frame synchronization digital input unused [^1]
- MPU6050 FSYNC (pin 1) connect to GND since external reference clock unused [^1]
- MPU6050 SDA (pin 24) and SCL (pin 23) pins need pull up resistors to VDD [^1]
- MPU6050 AD0 (pin 9) pulled to ground so that i2c address is 1101000 (shifted format it would be 0xD0 for write and 0xD1 for read) [^1]
- MPU6050 INT (pin 12) is an output pin to signal events. Since we're continuously polling, this is unused. [^1]
- MPU6050 AUX_DA (pin 6) and AUX_CL (pin 7) are for an external magnetometer and are unused. [^1]

[^1]: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf

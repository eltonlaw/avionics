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
- D1 LED for showing board is powered
- SAM-M10Q "power consumption of 37mW in continuous tracking mode with four concurrent GNSS" [^3]
- SAM-M10Q "The inrush current at startup can go up to 100 mA. Ensure that the external power supply is able to deliver up to 100 mA." [^3]
- SAM-M10Q "All the inputs have internal pull-up resistors in normal operation and can be left open if not used." [^3]
- SAM-M10Q V_IO and V_CC typical value is 3V with max of 3.6V and min of 2.7V
- SAM-M10Q "The inrush current at startup can go up to 100 mA" [^3]
- SAM-M10Q "all inputs have internal pull-up resisotrs in normal operation and can be left open if unused" [^3]
- SAM-M10Q "not protected against overvoltage or reversed voltages." [^3]

[^1]: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
[^2]: https://www.st.com/resource/en/datasheet/stm32g070cb.pdf
[^3]: https://content.u-blox.com/sites/default/files/documents/SAM-M10Q_DataSheet_UBX-22013293.pdf

# fc1

## Design

For the first iteration of this flight computer going to start with an atmega328p as the controller. Mainly because there's a lot of code + documentation supporting it from the Arduino ecosystem.

## fc1board

- LED connected to VCC to show when its powered
- For atmega328p
    - exposed UART and SPI pin headers for flashing & development
    - Using UART for debugging so an external crystal oscillator will be necessary, need to manually calibrate it. Factory calibration accuracy of the internal RC oscillator at 8.0MHz is +-2% with Vcc=3V,Temperature=25C and +-14% with Vcc=2.7V-5.5V,Temperature=-40C-125C.
    - Might need an external oscillator so that it can communicate with the MPU6050 faster which can do up to i2c@400kHz and atmel recommends at least 16 times higher than SCL frequency. 
    - external pull up resistors required for atmega 328p i2c pins, not specified in datasheet but 4.7k seemed like a commonly used amount when searching online
    - external 10 kiloohm pull up resistor for 1/PC6/RESET so it doesn't reset
- For MPU-6050, connected via i2c
    * Pin descriptions from datasheet
        - CLKIN/1, optional external ref clock input, connected to ground since unused
        - AUX_DA/6 and AUX_CL/7 not connected because not using external sensors
        - VLOGIC/8, digital I/O supply voltage connected to 3.3V with 10nF decoupling cap
        - AD0/9 i2c slave addres or spi serial data output; grounded
        - REGOUT/10, regulator filter capactior connection connected to 0.1uF decoupling cap
        - VDD/13, power supply voltage and digital I/O supply voltage. connected to 3.3V with 0.01uF decoupling cap
        - CPOUT/20 charge pump capacitor connection, connected to 2.2nF cap to ground
        - Not connected: 2, 3, 4, 5, 14, 15, 16,17, 19, 21, 22
    * copied some wiring from gy-521 (breakout board for MPU-6050)
        - INT/12 has 4.7k pull up resistor
    * If AD0=0 i2c address is 1101000 (0x68), if AD0=1 i2c address is 1101001 (0x69). No pull up resistor. Used for interacting with several sensors at once. Just have one so connecting to ground to use 0x68. 
    * AUX_DA, AUX_CL
    * VDD rated for -0.5V to +6V
    * Sensor needs some time to calibrate when it just gets started (and when it's stationary)
    * i2c operates at 400kHz
    * "Gyro, accelerometer and temperature measurement data are read-only registers accessed via the serial interface. Data from these registers may be read anytime"

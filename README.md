# quadcopter

    ├── avrlib: Shared utils
    ├── devtools: miscellaneous local dev stuff
    │   ├── blinky: Basic hex file to flash for testing hardware
    │   ├── cli: To replace x.sh
    │   └── serial_interactive.py: interactive uart with ipython
    ├── fc1: 
    │   ├── fc1board: Kicad files
    │   └── src: Firmware
    ├── kicad_sym: Shared symbols lib
    ├── sensor_calibrate: 3d printed test bench for simulating flight. ESP32 webserver connected to a servo.
    └── x.sh: Manages building/flashing/testing etc.

## Setup

Sets up environment vars and builds the CLI, a `./x` executable

```
./build_cli.sh
direnv allow .
```

### ESP32

Set `export ESP_IDF_PATH=...` in your shell profile

```
git clone https://github.com/espressif/esp-idf ${ESP_IDF_PATH}
${ESP_IDF_PATH}/install.sh esp32c3 --enable-pytest
${ESP_IDF_PATH}/tools/idf_tools.py install
```

### STM32

```
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib
```

[Download STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html#get-software) and add to \$PATH

## Flash with OpenOCD

```
# Start server
$ openocd -f ext/openocd/tc/board/st_nucleo_g0.cfg
# In separate terminal, connect to openocd server
$ telnet localhost 4444
> reset halt
> flash write_image erase ./fc3/build/fc3-G0.hex
> reset run
```
## Yocto

```
sudo pacman -S rpcsvc-proto chrpath cpio diffstat inetutils glibc
```

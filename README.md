# robotics

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

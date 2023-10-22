#pragma once

enum error_t {
    E_OK = 0,
    E_I2C_WRONG_DEVICE,
    // Catch all, discouraged
    E_UNKNOWN,
};
typedef enum error_t error_t;

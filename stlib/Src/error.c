#include "error.h"

const char* error_name(error_t err) {
    switch (err) {
        case E_HAL_OK:
            return "E_HAL_OK";
        case E_HAL_ERROR:
            return "E_HAL_ERROR";
        case E_HAL_BUSY:
            return "E_HAL_BUSY";
        case E_HAL_TIMEOUT:
            return "E_HAL_TIMEOUT";
        case E_OK:
            return "E_OK";
        case E_I2C_WRONG_DEVICE:
            return "E_I2C_WRONG_DEVICE";
        case E_WRONG_WHOAMI:
            return "E_WRONG_WHOAMI";
        case E_UNKNOWN:
            return "E_UNKNOWN";
        default:
            return "Error has no defined name";
    }
}

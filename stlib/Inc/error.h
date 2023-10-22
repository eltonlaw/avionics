#pragma once
#include "stm32g0xx.h"

enum error_t {
    E_HAL_OK = HAL_OK,
    E_HAL_ERROR = HAL_ERROR,
    E_HAL_BUSY = HAL_BUSY,
    E_HAL_TIMEOUT = HAL_TIMEOUT,
    E_OK,
    E_I2C_WRONG_DEVICE,
    // Catch all, discouraged
    E_UNKNOWN,
};
typedef enum error_t error_t;

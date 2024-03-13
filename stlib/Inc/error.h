#pragma once
#include "log.h"
#include "stm32g0xx.h"

enum error_t {
    E_HAL_OK = HAL_OK,
    E_HAL_ERROR = HAL_ERROR,
    E_HAL_BUSY = HAL_BUSY,
    E_HAL_TIMEOUT = HAL_TIMEOUT,
    E_OK,
    E_I2C_WRONG_DEVICE,
    E_WRONG_WHOAMI,
    E_INVALID_VALUE,
    // Catch all, discouraged
    E_UNKNOWN,
};
typedef enum error_t error_t;

const char* error_name(error_t err);

// FIXME: Add dump of relevant registers
#define panic(...) do { \
	log_fatal(__VA_ARGS__); \
	while(1); \
} while(0)

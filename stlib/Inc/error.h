#pragma once
#include "log.h"

enum error_t {
    E_OK = 0,
    // Catch all, discouraged
    E_UNKNOWN,
};
typedef enum error_t error_t;

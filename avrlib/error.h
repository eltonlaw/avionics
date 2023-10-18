#pragma once
#include "log.h"

enum avcerr_t {
    E_OK = 0,
    // Catch all, discouraged
    E_UNKNOWN,
};
typedef enum avcerr_t avcerr_t;

// FIXME: Add dump of relevant registers
#define panic(...) do { \
	log_fatal(__VA_ARGS__); \
	while(1); \
} while(0)

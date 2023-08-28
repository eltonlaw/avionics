#pragma once

enum avcerr_t {
    E_OK = 0,
};
typedef enum avcerr_t avcerr_t;


#include "log.h"

// FIXME: Add dump of relevant registers
#define panic(...) do { \
	log_fatal(__VA_ARGS__); \
	while(1); \
} while(0)

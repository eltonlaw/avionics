#pragma once

#include "log.h"

#define panic() do { \
	log_fatal("Panic at %s:%d", __FILE__, __LINE__); \
	while(1); \
} while(0)

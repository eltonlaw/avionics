#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stm32g0xx_hal.h"

// Define the log levels
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
} log_level_t;

// Define the log module
typedef struct {
    UART_HandleTypeDef *huart;
    log_level_t level;
    bool initialized;
    bool preamble;
} logger_t;

// Define the log module instance
extern logger_t logger;

// Define the log functions
void log_init(UART_HandleTypeDef *huart, log_level_t level);
void log_write(log_level_t level, const char *file, int line, const char *fmt, ...);
#define log_debug(...) log_write(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) log_write(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) log_write(LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_write(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_write(LOG_LEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__)

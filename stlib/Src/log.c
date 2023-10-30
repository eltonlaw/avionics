#include "log.h"

logger_t logger;

const char *log_level_strings[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

void log_init(UART_HandleTypeDef *huart, log_level_t level) {
    logger.huart = huart;
    logger.level = level;
    logger.initialized = true;
	logger.preamble = true;
}

void log_write(log_level_t level, const char *file, int line, const char *fmt, ...) {
    if (level > logger.level) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    char buf[256]; // Adjust buffer size as needed
    int offset = 0;
    if (logger.preamble) {
        offset = snprintf(buf, sizeof(buf), "[%s:%d] %s - ", file, line, log_level_strings[level]);
    }
    vsnprintf(buf + offset, sizeof(buf) - offset, fmt, args);

    HAL_UART_Transmit(logger.huart, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
    va_end(args);
}

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
    va_list args;
    va_start(args, fmt);
    char buf[256]; // Adjust buffer size as needed
                   //
    if (level < logger.level) {
        return;
    }
    int offset = 0;
    if (logger.preamble) {
        offset = snprintf(buf, sizeof(buf), "[%s:%d] %s - ", file, line, log_level_strings[level]);
    }
    vsnprintf(buf + offset, sizeof(buf) - offset, fmt, args);
    va_end(args);

    HAL_UART_Transmit(logger.huart, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
}

/* Base function for creating event data
 * Sample structure, where event id is a 3 char letter
 * <EVENT_ID> data1,data2,data3
 * */
void log_event(const char *event_id, const char *fmt, ...) {
    char buf[256];
    va_list args;
    va_start(args, fmt);
    int offset = snprintf(buf, sizeof(buf), "%s ", event_id);
    vsnprintf(buf + offset, sizeof(buf) - offset, fmt, args);
    va_end(args);

    HAL_UART_Transmit(logger.huart, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
}

void event_imu_read(float ax, float ay, float az, float gx, float gy, float gz) {
    log_event("IMU", "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", ax, ay, az, gx, gy, gz);
}

void event_gps_read(double latitude, double longitude, float altitude) {
    log_event("GPS", "%.6f,%.6f,%.6f\n", latitude, longitude, altitude);
}

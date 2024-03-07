#pragma once
#include "error.h"

typedef struct {
    double accel_x;
    double accel_y;
    double accel_z;
    double temperature;
    double gyro_x;
    double gyro_y;
    double gyro_z;
} imu_data_t;

typedef struct {
    double angle_x;
    double angle_y;
    double angle_z;
} state_t;

error_t update_state(state_t* s, imu_data_t* imu_data, double delta_ticks);

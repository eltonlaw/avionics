#include "control.h"
#include <stdint.h>
#include <math.h>

#define ALPHA 0.98

error_t update_state(state_t *s, imu_data_t* imu_data, double delta_ticks) {
    const double dt = delta_ticks / 1000;
    // Integrate angular velocity to get angular position
    double ang_x = s->ang_x + imu_data->gyro_x * dt;
    double ang_y = s->ang_y + imu_data->gyro_y * dt;
    double ang_z = s->ang_z + imu_data->gyro_z * dt;

    // angle from accelerometer (assumes gravity is only force)
    double accel_ang_x = atan2(imu_data->accel_y, sqrt(pow(imu_data->accel_x,
                    2) + pow(imu_data->accel_z, 2)));
    double accel_ang_y = atan2(-imu_data->accel_x,
            sqrt(pow(imu_data->accel_y, 2) + pow(imu_data->accel_z, 2)));

    // gyro readings are prone to drift. complementary filter combines
    // gyro-derived angle and accel-derived angle
    s->ang_x = ALPHA * ang_x + (1 - ALPHA) * accel_ang_x;
    s->ang_y = ALPHA * ang_y + (1 - ALPHA) * accel_ang_y;
    s->ang_z = ang_z;

    // Integrate linear acceleration twice to get position
    s->vel_x += imu_data->accel_x * dt;
    s->vel_y += imu_data->accel_y * dt;
    s->vel_z += imu_data->accel_z * dt;
    s->pos_x += s->vel_x * dt;
    s->pos_y += s->vel_y * dt;
    s->pos_z += s->vel_z * dt;

    return E_OK;
}

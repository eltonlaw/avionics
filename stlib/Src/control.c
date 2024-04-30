#include "control.h"
#include <stdint.h>

error_t update_state(state_t *s, imu_data_t* imu_data, double delta_ticks) {
    const double dt = delta_ticks / 1000;
    // Integrate angular velocity to get angular position
    s->ang_x = s->ang_x + imu_data->gyro_x * delta_ticks;
    s->ang_y = s->ang_y + imu_data->gyro_y * delta_ticks;
    s->ang_z = s->ang_z + imu_data->gyro_z * delta_ticks;

    // Integrate linear acceleration twice to get position
    s->vel_x = s->vel_x + imu_data->accel_x * delta_ticks;
    s->vel_y = s->vel_y + imu_data->accel_y * delta_ticks;
    s->vel_z = s->vel_z + imu_data->accel_z * delta_ticks;
    s->pos_x = s->pos_x + s->vel_x * delta_ticks;
    s->pos_y = s->pos_y + s->vel_y * delta_ticks;
    s->pos_z = s->pos_z + s->vel_z * delta_ticks;


    return E_OK;
}

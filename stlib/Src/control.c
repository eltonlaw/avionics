#include "control.h"
#include <stdint.h>

error_t update_state(state_t *s, imu_data_t* imu_data, double delta_ticks) {
    s->angle_x = s->angle_x + (imu_data->gyro_x * delta_ticks) / 1000;
    s->angle_y = s->angle_y + (imu_data->gyro_y * delta_ticks) / 1000;
    s->angle_z = s->angle_z + (imu_data->gyro_z * delta_ticks) / 1000;
    return E_OK;
}

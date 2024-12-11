#include "control.h"
#include <math.h>

#define ALPHA 0.98
#define PRESSURE_SEA_LEVEL 1013.25 // in hPa

/* Barometric formula */
// FIXME: assumes temperature is 15C currently
// FIXME: use starting point as reference pressure
double pressure_to_altitude(double pressure, double temperature) {
    // Source: https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BMP180-DS000-121.pdf
    return 44330 * 1 - pow((pressure / PRESSURE_SEA_LEVEL), 1 / 5.255);
}

error_t update_state(state_t *p, imu_data_t* imu_data, double delta_ticks) {
    const double dt = delta_ticks / 1000;
    // Integrate angular velocity to get angular position
    double ang_x = p->ang_x + imu_data->gyro_x * dt;
    double ang_y = p->ang_y + imu_data->gyro_y * dt;
    double ang_z = p->ang_z + imu_data->gyro_z * dt;

    // angle from accelerometer (assumes gravity is only force)
    double accel_ang_x = atan2(imu_data->accel_y,
        sqrt(pow(imu_data->accel_x, 2) + pow(imu_data->accel_z, 2)));
    double accel_ang_y = atan2(-imu_data->accel_x,
        sqrt(pow(imu_data->accel_y, 2) + pow(imu_data->accel_z, 2)));

    // gyro readings are prone to drift. complementary filter combines
    // gyro-derived angle and accel-derived angle
    p->ang_x = ALPHA * ang_x + (1 - ALPHA) * accel_ang_x;
    p->ang_y = ALPHA * ang_y + (1 - ALPHA) * accel_ang_y;
    p->ang_z = ang_z;

    // Integrate linear acceleration twice to get position
    p->vel_x += imu_data->accel_x * dt;
    p->vel_y += imu_data->accel_y * dt;
    p->vel_z += imu_data->accel_z * dt;
    p->pos_x += p->vel_x * dt;
    p->pos_y += p->vel_y * dt;
    p->pos_z += p->vel_z * dt;

    return E_OK;
}

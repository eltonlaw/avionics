#pragma once
#include <vector>
#include <string>

namespace sim {

struct SensorData {
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
};
std::vector<SensorData> load_sensor_data(const std::string& filepath);

} // namespace sim

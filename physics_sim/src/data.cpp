#include <fstream>
#include <sstream>
#include <iostream>
#include "data.hpp"

namespace sim {

std::vector<SensorData> load_sensor_data(const std::string& filepath) {
    std::vector<SensorData> data;
    std::ifstream file(filepath);
    std::string line;

    if (!file) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return data;
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        SensorData sd;
        char dummy;
        for (int i = 0; i < 4; ++i) {
            iss >> dummy;
        }
        if (!(iss >> sd.accel_x >> dummy >> sd.accel_y >> dummy >> sd.accel_z
                  >> dummy >> sd.gyro_x >> dummy >> sd.gyro_y >> dummy >> sd.gyro_z)) {
            continue;  // skip malformed lines
        }
		data.push_back(sd);
    }
    return data;
}

} // namespace sim

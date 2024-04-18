#include <iostream>
#include "render.hpp"
#include "data.hpp"

using namespace sim;

int main(int argc, char * argv[]) {
    (void) argc;
    (void) argv;
    auto data = sim::load_sensor_data("../../usart.log");
    for (const auto& sensor : data) {
        std::cout << "Accel (X, Y, Z): (" << sensor.accel_x << ", " << sensor.accel_y << ", " << sensor.accel_z << ")"
                  << " | Gyro (X, Y, Z): (" << sensor.gyro_x << ", " << sensor.gyro_y << ", " << sensor.gyro_z << ")\n";
    }
	return 0;
    // return sim::render();
}

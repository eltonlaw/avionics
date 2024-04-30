#include "unity.h"
#include "control.h"

// void test_update_state(void) {
//     state_t s = {0, 0, 0};
//     imu_data_t imu_data = {1, 2, 3};
//     double delta_ticks = 1000;
//     update_state(&s, &imu_data, delta_ticks);
//     TEST_ASSERT_EQUAL(1, s.angle_x);
//     TEST_ASSERT_EQUAL(2, s.angle_y);
//     TEST_ASSERT_EQUAL(3, s.angle_z);
// }
//
void test_add(void) {
    TEST_ASSERT_EQUAL(3, add(1, 2));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_add_two_numbers);
    return UNITY_END();
}

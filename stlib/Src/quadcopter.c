#include "log.h"
#include "quadcopter.h"

void qc_init(
    qc_state_t *st,
    I2C_HandleTypeDef *hi2c1,
    I2C_HandleTypeDef *hi2c2,
    UART_HandleTypeDef *huart2,
    UART_HandleTypeDef *huart3,
    TIM_HandleTypeDef *htim3
) {
    error_t err;

    HAL_TIM_Base_Start(htim3);
    __HAL_TIM_SET_COUNTER(htim3, 0);
    st->htim3 = htim3;

    for (int i = 0; i < 10; i++) {
      HAL_Delay(200);
      HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    }

    log_init(huart2, LOG_LEVEL_DEBUG);
    log_info("Logging initialized\n");

    st->mpu6050_cfg.i2cx = hi2c1;
    st->mpu6050_cfg.timx = htim3;
    st->mpu6050_cfg.gyro_range = MPU6050_GYRO_RANGE_250;
    st->mpu6050_cfg.accel_range = MPU6050_ACCEL_RANGE_2;


    if (E_OK == (err = mpu6050_init_w_retry(&st->mpu6050_cfg, 4))) {
        log_info("Initialized MPU6050 accel_scaler=%lf, gyro_scaler=%lf, accel_offset=(x:%lf, y:%lf, z:%lf), gyro_offset=(x:%lf, y:%lf, z:%lf)\n",
            st->mpu6050_cfg.accel_scaler,
            st->mpu6050_cfg.gyro_scaler,
            st->mpu6050_cfg.offset.accel_x,
            st->mpu6050_cfg.offset.accel_y,
            st->mpu6050_cfg.offset.accel_z,
            st->mpu6050_cfg.offset.gyro_x,
            st->mpu6050_cfg.offset.gyro_y,
            st->mpu6050_cfg.offset.gyro_z);
    } else {
        panic("Failed to initialize MPU6050: %d\n", err);
    }

    // sam_m10q_cfg.uartx = &huart3;
    // sam_m10q_init(&sam_m10q_cfg);

    // bme280_cfg.i2cx = &hi2c2;
    // bme280_cfg.addr = BME280_ADDR_0;
    // if (E_OK == (err = bme280_init(&bme280_cfg))) {
    //     log_info("Initialized BME280\n");
    // } else {
    //     panic("Failed to initialize BME280: %d\n", err);
    // }
    //
    log_info("Initialization complete, starting main loop\n");
    HAL_Delay(3000);
    __HAL_TIM_SET_COUNTER(htim3, 0);
}

void qc_update(qc_state_t* st) {
    mpu6050_read(&st->mpu6050_cfg, &st->imu_data);
    double delta_ticks = __HAL_TIM_GET_COUNTER(st->htim3);
    __HAL_TIM_SET_COUNTER(st->htim3, 0);

    /* FIXME: Incorporate into altitude calculations*/
    // bme280_read(&bme280_cfg, &bme280_data);
    // log_info("Temperature: %lf, Pressure: %lf, Humidity: %lf\n",
    //         bme280_data.temperature, bme280_data.pressure, bme280_data.humidity);

    update_pose(&st->pose, &st->imu_data, delta_ticks);

    event_imu_read(st->imu_data.accel_x, st->imu_data.accel_y, st->imu_data.accel_z,
            st->imu_data.gyro_x, st->imu_data.gyro_y, st->imu_data.gyro_z);
    // sam_m10q_read(&sam_m10q_cfg, &sam_m10q_data);
}

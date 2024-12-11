#include "log.h"
#include "quadcopter.h"

void qc_init(
    qc_sys_t *sys,
    I2C_HandleTypeDef *hi2c1,
    I2C_HandleTypeDef *hi2c2,
    UART_HandleTypeDef *huart2,
    UART_HandleTypeDef *huart3,
    TIM_HandleTypeDef *htim3
) {
    error_t err;

    HAL_TIM_Base_Start(htim3);
    __HAL_TIM_SET_COUNTER(htim3, 0);
    sys->htim3 = htim3;

    for (int i = 0; i < 10; i++) {
      HAL_Delay(200);
      HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    }

    log_init(huart2, LOG_LEVEL_DEBUG);
    log_info("Logging initialized\n");

    sys->mpu6050_cfg.i2cx = hi2c1;
    sys->mpu6050_cfg.timx = htim3;
    sys->mpu6050_cfg.gyro_range = MPU6050_GYRO_RANGE_250;
    sys->mpu6050_cfg.accel_range = MPU6050_ACCEL_RANGE_2;


    if (E_OK == (err = mpu6050_init_w_retry(&sys->mpu6050_cfg, 4))) {
        log_info("Initialized MPU6050 accel_scaler=%lf, gyro_scaler=%lf, accel_offset=(x:%lf, y:%lf, z:%lf), gyro_offset=(x:%lf, y:%lf, z:%lf)\n",
            sys->mpu6050_cfg.accel_scaler,
            sys->mpu6050_cfg.gyro_scaler,
            sys->mpu6050_cfg.offset.accel_x,
            sys->mpu6050_cfg.offset.accel_y,
            sys->mpu6050_cfg.offset.accel_z,
            sys->mpu6050_cfg.offset.gyro_x,
            sys->mpu6050_cfg.offset.gyro_y,
            sys->mpu6050_cfg.offset.gyro_z);
    } else {
        panic("Failed to initialize MPU6050: %d\n", err);
    }

    // sam_m10q_cfg.uartx = &huart3;
    // sam_m10q_init(&sam_m10q_cfg);

    sys->bme280_cfg.i2cx = &hi2c2;
    sys->bme280_cfg.addr = BME280_ADDR_0;
    if (E_OK == (err = bme280_init(&sys->bme280_cfg))) {
        log_info("Initialized BME280\n");
    } else {
        panic("Failed to initialize BME280: %d\n", err);
    } log_info("Initialization complete, starting main loop\n");
    HAL_Delay(3000);
    __HAL_TIM_SET_COUNTER(htim3, 0);
}

void qc_update(qc_sys_t* sys) {
    mpu6050_read(&sys->mpu6050_cfg, &sys->imu_data);
    double delta_ticks = __HAL_TIM_GET_COUNTER(sys->htim3);
    __HAL_TIM_SET_COUNTER(sys->htim3, 0);

    /* FIXME: Incorporate into altitude calculations*/
    bme280_read(&sys->bme280_cfg, &sys->bme280_data);
    log_info("T: %lf, P: %lf, H: %lf\n",
            sys->bme280_data.temperature,
            sys->bme280_data.pressure,
            sys->bme280_data.humidity);

    update_state(&sys->state, &sys->imu_data, delta_ticks);

    event_imu_read(sys->imu_data.accel_x,sys->imu_data.accel_y,
            sys->imu_data.accel_z, sys->imu_data.gyro_x, sys->imu_data.gyro_y,
            sys->imu_data.gyro_z);
    // sam_m10q_read(&sam_m10q_cfg, &sam_m10q_data);
}

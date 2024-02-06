#include "stm32g0xx_hal.h"
#include "stdio.h"
#include "log.h"
#include "mpu6050.h"

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;

void SystemClock_Config() {
  // Zero init structs
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
}

int main(void) {
  mpu6050_cfg_t mpu6050_cfg;
  mpu6050_data_t mpu6050_data;
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();

  log_init(&huart2, LOG_LEVEL_INFO);
  mpu6050_init(&mpu6050_cfg, &hi2c1);
  log_info("Initialized MPU6050 accel_scaler=%lf, gyro_scaler=%lf, accel_offset=(x:%lf, y:%lf, z:%lf), gyro_offset=(x:%lf, y:%lf, z:%lf)",
      mpu6050_cfg.accel_scaler,
      mpu6050_cfg.gyro_scaler,
      mpu6050_cfg.offset.accel_x,
      mpu6050_cfg.offset.accel_y,
      mpu6050_cfg.offset.accel_z,
      mpu6050_cfg.offset.gyro_x,
      mpu6050_cfg.offset.gyro_y,
      mpu6050_cfg.offset.gyro_z);

  while (1) {
    mpu6050_read(&mpu6050_cfg, &mpu6050_data);
    log_info("Accel(X:%lfG, Y:%lfG, Z:%lfG), Temp:%lf, Gyro(X:%lf, Y:%lf, Z:%lf)\r\n",
        mpu6050_data.accel_x, mpu6050_data.accel_y, mpu6050_data.accel_z,
        mpu6050_data.temperature,
        mpu6050_data.gyro_x, mpu6050_data.gyro_y, mpu6050_data.gyro_z);
  }
}

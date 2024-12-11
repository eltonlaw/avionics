#include "bme280.h"
#include "control.h"
#include "mpu6050.h"
#include "sam_m10q.h"

#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_conf.h"

#define LED_GREEN_Pin GPIO_PIN_5
#define LED_GREEN_GPIO_Port GPIOA

typedef struct {
    mpu6050_cfg_t mpu6050_cfg;
    bme280_cfg_t bme280_cfg;
    imu_data_t imu_data;
    bme280_data_t bme280_data;
    pose_t pose;
    sam_m10q_cfg_t sam_m10q_cfg;
    sam_m10q_data_t sam_m10q_data;
    TIM_HandleTypeDef *htim3;
} qc_state_t;

void qc_init(
    qc_state_t *state,
    I2C_HandleTypeDef *hi2c1,
    I2C_HandleTypeDef *hi2c2,
    UART_HandleTypeDef *huart2,
    UART_HandleTypeDef *huart3,
    TIM_HandleTypeDef *htim3);

void qc_update(qc_state_t *state);

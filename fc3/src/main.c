#include <string.h>
#include "stm32g0xx_hal_conf.h"
#if defined FC3_G0
  #include "stm32g0xx_hal.h"
#elif defined FC3_F1
  #include "stm32f1xx_hal.h"
#endif

UART_HandleTypeDef huart2;

void Error_Handler(void) {
  __disable_irq();
  while (1) { }
}

static void MX_USART2_UART_Init(void) {
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
    Error_Handler();
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    Error_Handler();
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    Error_Handler();
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
    Error_Handler();
}

int main(void) {
  HAL_Init();
  MX_USART2_UART_Init();

  char buf[] = "hello world\n";
  while(1) {
    HAL_UART_Transmit(&huart2, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
    HAL_Delay(1000);
  }
  return 0;
}

/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include <stdio.h>

#define GPIO_OUTPUT_IO_0 18
#define GPIO_OUTPUT_IO_1 19
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))

static const char* TAG = __FILE__;

void vTaskFunction(void *pvParams) {
    for (;;) {
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Configuring GPIO pins...");
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    uint8_t led_state = 0;
    ESP_LOGI(TAG, "initialized successfully");
    int cnt = 0;
    while(1) {
        ESP_LOGI(TAG, "blinking %d", cnt++);
        led_state = cnt % 2;
        gpio_set_level(GPIO_OUTPUT_IO_0, !led_state);
        gpio_set_level(GPIO_OUTPUT_IO_1, led_state);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

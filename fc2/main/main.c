#include "esp_http_server.h"
#include "esp_log.h" #include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

static const char *TAG = "main";

esp_err_t get_handler(httpd_req_t *req) {
    const char resp[] = "URI GET Response";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

httpd_uri_t uri_get = {
    .uri = "uri",
    .method = HTTP_GET,
    .handler = get_handler,
    .user_ctx = NULL,
};


httpd_handle_t webserver_start(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(httpd_start(&server, &config));
    httpd_register_uri_handler(server, &uri_get);
    ESP_LOGI(TAG, "Webserver started %hu", config.server_port);
    return server;
}

void webserver_stop(httpd_handle_t server) {
    if (server) {
        httpd_stop(server);
    }
}

void app_main(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    httpd_handle_t server = webserver_start();
    while(1) {
        ESP_LOGI(TAG, "Waiting for responses");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    webserver_stop(server);
}

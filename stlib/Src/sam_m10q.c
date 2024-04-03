/* sam_m10q.c
 *
 */
#include "log.h"
#include "sam_m10q.h"

error_t sam_m10q_init(sam_m10q_cfg_t* cfg) {
    log_info("sam_m10q_init");
    return E_OK;
}

error_t sam_m10q_read(sam_m10q_cfg_t* cfg, sam_m10q_data_t *data) {
    char rx_data;
    char buffer[256];
    uint16_t index = 0;

    while (1) {
        HAL_UART_Receive(cfg->uartx, (uint8_t *)&rx_data, 1, HAL_MAX_DELAY);

        if (rx_data == '\n') {
            buffer[index] = '\0'; // Null-terminate the string
            log_info("%s\n", buffer);
            index = 0;
            continue;
        }

        buffer[index++] = rx_data;
        if (index >= 256 - 1) {
            buffer[index] = '\0';
            log_info("%s ...(Messaged truncated)\n", buffer);
            index = 0;
            continue;
        }
    }
    return E_OK;
}

/*
 * sam_m10q.c
 *
 * - RAM receiver configuration is configured on startup each time, power-down and reset clears it.
 * - Battery-backed RAM (BBR) is also used to store config, when applying runtime configs, it should be applyed to RAM and to BBR
 * - "Neither handshaking signals nor hardware flow control signals are supported" for UART, so use a bus that doesn't have CTS and RTS.
 */
#include "log.h"
#include "sam_m10q.h"

/*
 * - Poll request type
 * - Message returns a UBX-ACK-NACK
 * - You can use wildcard by specifying the group specifier but put 0xffff in bits 0-15
 * - message is limited to a max of 64 key ids
 * - byte[0] (U1): version
 * - byte[1] (U1): layer
 * - byte[2:3] (U2): position
 * - byte[4:4+n*4] (U4): repeated key ids
 * */
error_t ubx_valget(uint32_t key_id) {
    // message is limited to a max of 64 key ids
    // uint32_t key_ids[64];
    return E_OK;
}

/* The checksum is calculated over the message, starting and including the
 * class ﬁeld up until, but excluding, the checksum ﬁelds */
error_t ubx_msg_update_ck(uint8_t len, uint8_t* frame) {
    uint8_t ck_a_i = len - 1;
    uint8_t ck_b_i = len - 2;
    // start at 2 to skip device message header, end when CK_A reached
    for (uint8_t i=2; i < ck_b_i; i++) {
        frame[ck_a_i] = frame[i] + frame[ck_a_i];
        frame[ck_b_i] = frame[ck_b_i] + frame[ck_a_i];
    }
    return E_OK;
}

error_t ubx_msg_new(uint8_t len, uint8_t* payload) {
    return E_OK;
}

/*
 * Header: 0xb6 0x62
 * Class: 0x0a
 * Id: 0x04
 * Length (bytes): 0
 * Payload: This message has no payload
 * Checksum: CK_A CK_B
 * */
error_t ubx_mon_ver(void) {
    error_t err;
    uint8_t frame[] = {0xb5, 0x62, 0x0a, 0x04, 0x00, 0x00, 0x00, 0x00};
    err = ubx_msg_update_ck(8, frame);
    return err;
}

error_t sam_m10q_init(sam_m10q_cfg_t* cfg) {
    // FIXME: VALSET info message configuration
    // FIXME: VALGET info message
    // FIXME: VALSET enable i2c
    // FIXME: VALSET set i2c address
    // FIXME: Configure NMEA output configuration for multi-gnss
    // FIXME: Set a higher update rate
    // FIXME: Check if signal lock found
    // FIXME: Configure RTK networks
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

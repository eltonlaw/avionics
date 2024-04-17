#pragma once

#include "stm32g0xx.h"
#include "error.h"

#define SAM_M10Q_MESSAGE_HEADER 0xb562
#define SAM_M10Q_CLASS_ID_UBX_CFG_VALGET 0x068b
#define SAM_M10Q_CLASS_ID_UBX_MON_VER 0x0a04

#define SAM_M10Q_ADDR (0x42 << 1)
#define SAM_M10Q_NUM_BYTES_TO_READ_H 0xFD
#define SAM_M10Q_NUM_BYTES_TO_READ_L 0xFE
#define SAM_M10Q_VALUE 0xFF

/* ublox datatypes from interface description
 *
 * L: single-bit boolean (true=1, false=0) stored as U1
 * U1/2/4/8: unsigned little-endian integer 8/16/32/64-bit
 * X1/2/4/8: unsigned little-endian integers of 8/16/32/64-bit
 * */
#define SAM_M10Q_CFG_KEY_I2C_ADDRESS 0x20510001 // U1
#define SAM_M10Q_CFG_KEY_I2C_ENABLED 0x10510003 // L
#define SAM_M10Q_CFG_KEY_INFMSG_UBX_I2C 0x20920001 // X1
#define SAM_M10Q_CFG_KEY_INFMSG_UBX_UART1 0x20920002 // X1
#define SAM_M10Q_CFG_KEY_INFMSG_NMEA_I2C 0x20920006 // X1
#define SAM_M10Q_CFG_KEY_INFMSG_NMEA_UART1 0x20920007 // X1

#define SAM_M10Q_CFG_VAL_INFMSG_ERROR 0x01
#define SAM_M10Q_CFG_VAL_INFMSG_WARNING 0x02
#define SAM_M10Q_CFG_VAL_INFMSG_NOTICE 0x04
#define SAM_M10Q_CFG_VAL_INFMSG_TEST 0x08
#define SAM_M10Q_CFG_VAL_INFMSG_DEBUG 0x10

typedef struct {
    I2C_HandleTypeDef* i2cx;
	UART_HandleTypeDef* uartx;
} sam_m10q_cfg_t;

typedef struct {
    uint8_t status;
} sam_m10q_data_t;

error_t sam_m10q_init(sam_m10q_cfg_t* cfg);
error_t sam_m10q_read(sam_m10q_cfg_t* cfg, sam_m10q_data_t *data);
error_t ubx_msg_update_ck(uint8_t len, uint8_t* frame);
error_t ubx_msg_new(uint8_t len, uint8_t* payload);
error_t ubx_mon_ver(void);

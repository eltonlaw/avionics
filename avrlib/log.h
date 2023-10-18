/* USART logger */
#pragma once
#include "usart.h"

#define log_init() usart_init()
#define log_debug(...) usart_log(__FILE__, __LINE__,"[DEBUG] "  __VA_ARGS__)
#define log_info(...) usart_log(__FILE__, __LINE__,"[INFO] "  __VA_ARGS__)
#define log_error(...) usart_log(__FILE__, __LINE__,"[ERROR] "  __VA_ARGS__)
#define log_fatal(...) usart_log(__FILE__, __LINE__, __VA_ARGS__)

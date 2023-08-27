/* USART logger */
#pragma once
#include "usart.h"

#define log_init() usart_init()
#define log_info(...) usart_print_formatted("[INFO] " __VA_ARGS__)
#define log_fatal(...) usart_print_formatted("[FATAL] " __VA_ARGS__)

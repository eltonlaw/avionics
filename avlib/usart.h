#pragma once

void usart_init(void);
void usart_send_byte(uint8_t);
uint8_t usart_recv_byte(void);
void usart_print_str(const char s[]);
void usart_log(const char *file, int line, const char *format, ...);

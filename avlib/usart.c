#include <avr/io.h>
#include "usart.h"
#include <util/setbaud.h>

/* USART initialization
 *
 * Needed to start comms. Need to set baud rate, frame
 * rate, enable the sendter/receiver and optionally set
 * the global interrupt flag
 * */
void usart_init() {
    // Auto-configured by preprocessor when
    // including util/setbaud.h when BAUD and
    // F_CPU are defined
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif
    // Enable sender/receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    // 8 data bits 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

uint8_t usart_recv_byte(void) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

void usart_send_byte(uint8_t data) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data;
}

void usart_print_str(const char s[]) {
    uint8_t i = 0;
    while(s[i]) {
        usart_send_byte(s[i]);
        i++;
    }
}

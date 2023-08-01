#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

int main() {
    char c;
    usart_init();
    DDRB |= ((1 << PINB0) & (1 << PINB1));

    usart_print_str("Starting fc1!\r\n");

    while (1) {
        // Toggle PB0/PB1
        PORTB ^= ((1 << PINB0) & (1 << PINB1));

        // basic echo
        c = usart_recv_byte();
        usart_send_byte(c);

        _delay_ms(1000);
    }
    return 0;
}

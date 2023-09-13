#include <avr/io.h>
#include <util/delay.h>

int main() {
    DDRB |= (1 << PINB0);
    DDRB |= (1 << PINB1);

    while (1) {
        PORTB ^= (1 << PINB0);  // Toggle only PB0
        PORTB ^= (1 << PINB1);  // Toggle only PB0
        _delay_ms(1000);
    }
    return 0;
}

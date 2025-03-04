#include <avr/io.h>

void main(void)
{
    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);
    while(1){}
}
#include <avr/io.h>

int main(void)
{
    DDRB |= (1 << PB1);
    PORTB &= ~(1 << PB1);
    while (1)
    {
        for (long nb = 0; nb < 240000; nb++);
        PORTB ^= (1 << PB1);
    }
}
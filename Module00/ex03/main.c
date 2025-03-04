#include <avr/io.h>
#include <util/delay.h>

void main(void)
{
    DDRB |= (1 << PB0);
    DDRD &= ~(1 << PD2);
    while (1)
    {
        while ((PIND & (1 << PD2))){} 
        PORTB ^= (1 << PB0); // When I switch button I change mode
         _delay_ms(20);
        while (!(PIND & (1 << PD2))){}
    }
}
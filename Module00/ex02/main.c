#include <avr/io.h>
#include <util/delay.h>

void main(void)
{
    DDRB |= (1 << PB0);
    DDRD &= ~(1 << PD2); /* (0000 0001 << 2) = ~ 0000 0100 */
    /* 0000 0000 & 0000 0100 = 0000 0000 */
    while (1)
    {
        if ((PIND & (1 << PD2)))
        {
            PORTB &= ~(1 << PB0);
            _delay_ms(50);
        }
        else
        PORTB |= (1 << PB0);
        /* 0000 0001 << 2 = 0000 0100 */
        /* 0000 0000 & 0000 0100 = 0000 0000*/
    }
}
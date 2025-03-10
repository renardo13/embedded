#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* We used an "is_released" variable to turn the light up 1/2 times otherwise the logical change of the ISC00 bit will change for each transition. 
It means when the button is pressed and released, and vice-versa. */

ISR(INT0_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (is_pressed)                /* Switching LED output only when button is pressed, 
                                    if you don't do that, the light will turn off when you release the button  */
		PORTB ^= (1 << PB0);
	_delay_ms(20);                 /* Neutralizing bounce effect */

}

int main(void)
{
	SREG |= (1 << 7);                   /* set Global Interrupt Enable bit in SREG*/
	EIMSK |= (1 << INT0);    /* Enable INT0 Interrupt */
	EICRA |= (1 << ISC00);   /* This is essentiel because otherwise the button will trigger many times if the button is keeping pressed. 
                                Any logical change on INT0 generates an interrupt request */
	DDRB |= (1 << PB0); // The led in output mode
	while (1) {}
}
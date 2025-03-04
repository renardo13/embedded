#include <avr/io.h>
#include <util/delay.h>

/* All the bits of PORTB represent the leds,
    one bit one led. So we can controlled all the led together
only with the value of PORTB. We compared the bits of value with 15 who is 1111 in binary.
Considere that the 4 first bit are zero. */


int is_pressed(int regist_id, int button)
{
	if(!(regist_id & (1 << button)))
		return(1);
	return(0);
}

int	main(void)
{
	int	value;

	value = 0;
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	DDRD &= ~((1 << PD2) | (1 << PD4));
	while (1)
	{
		if (is_pressed(PIND, PD2) && value < 15)
		{
			value++;
			while (is_pressed(PIND, PD2));
		}
		if (is_pressed(PIND, PD4) && value > 0)
		{
			value--;
            while (is_pressed(PIND, PD4));
		}
		PORTB = (value & 8) << 1 | (value & 7);
		/* We use the mask 7 to compare "111" with the value and light on the good led.
		For the led D4 we have to isolate the comparison with 8 "1000" and shift the bit to the left. */
		
	}	
	return (0);
}

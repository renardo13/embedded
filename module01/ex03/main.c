#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

int is_pressed(int regist_id, int button)
{
	if(!(regist_id & (1 << button)))
		return(1);
	return(0);
}

int main(void)
{
    DDRB |= (1 << PB1); // For the led

    TCCR1A |= (1 << COM1A1);             // Clear on compare match
    TCCR1B |= (1 << WGM13);              // Mode 8 "PWM, phase and frequency correct "
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set the prescaler

    ICR1 = F_CPU / (2 * 1 * 1024);

    OCR1A = ICR1 / 10;

    int val = ICR1 / 10; // To get the proportion to have 10% of the 1hz duty cycle
    while (1)
    {
        if (is_pressed(PIND, PD2) && OCR1A < ICR1)
        {
            while(is_pressed(PIND, PD2));
            OCR1A += val;
        }
        if (is_pressed(PIND, PD4) && OCR1A > val)
        {
            while(is_pressed(PIND, PD4));
            OCR1A -= val;
        }
        _delay_ms(20);
    }
}

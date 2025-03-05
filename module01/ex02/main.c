#include <avr/io.h>

# define F_CPU 16000000UL

int main(void)
{
    DDRB |= (1 << PB1);
    
    TCCR1A |= (1 << COM1A1) | (1 << WGM11); // COM1A11 to clear OC1A on compare match (set output to low level)
    TCCR1B |= (1 << WGM13) | (1 << CS10) | (1 << CS12); // WGM{11 & 13} to set Fast PWM mode. CS{10, 12} to set prescaler to 1024

    ICR1 = F_CPU / (2 * 1 * 1024); // This one is to set the counting cycle

    OCR1A = ICR1 / 10; // The new value to know when we set at 0 or 1 OC1A value, it switch of or on the led when it match value

    while (1){}
}

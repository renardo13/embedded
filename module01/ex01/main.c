#include <avr/io.h>

# define F_CPU 16000000UL

int main(void)
{
    DDRB |= (1 << PB1);
    
    TCCR1A |= (1 << COM1A0) | (1 << WGM10) | (1 << WGM11); // COM1A10 to toogle OC1A on compare match
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS12); // WGM{10, 11, 12 & 13} to set Fast PWM mode. CS{10, 12} to set prescaler to 1024

    OCR1A = F_CPU / (2 * 1 * 1024);

    while (1){}
}

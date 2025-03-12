#include "../uart.h"

// Function to initialize Timer1
void set_timer()
{
    SREG |= (1 << 7); // Enable global interrupts

    TCCR1B |= (1 << WGM12) | (1 << CS12); /* CTC mode with OCR1A as TOP value + prescaler 256 */
    OCR1A = ((F_CPU / PRESCALER / 1000) * 20);        /* Timer frequency */
    TIMSK1 |= (1 << OCIE1A);              // Enable Timer1 compare interrupt
}

void adc_init(void)
{
    ADMUX |= (1 << REFS0);                                // Set AVCC as voltage reference
    ADMUX |= (0 << ADLAR);                                // Set 8bits only precision
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler = 128 (appropriate for 16 MHz clock)
    ADCSRA |= (1 << ADEN);                                // Enable ADC
}

void start_adc_conversion(void)
{
    ADCSRA |= (1 << ADSC);       // Start the conversion
    while (ADCSRA & (1 << ADSC)) // Wait for the conversion to finish
        ;
}

// ISR for Timer1
ISR(TIMER1_COMPA_vect)
{
    start_adc_conversion();
    print_hex(ADC);
    uart_printstr("\b\b");
}

int main(void)
{
    uart_init();                  // Initialize UART
    set_timer();                  // Initialize Timer1
    adc_init(); // Initialize ADC

    while (1)
    {
    }
    return 0;
}

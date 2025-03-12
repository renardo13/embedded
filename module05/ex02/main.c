#include "../uart.h"

void adc_init(void)
{
    ADMUX |= (1 << REFS0);                                // Set AVCC as voltage reference
    ADMUX &= ~(1 << ADLAR);                                // Set 10bits only precision
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler = 128 (appropriate for 16 MHz clock)
    ADCSRA |= (1 << ADEN);                                // Enable ADC
}

void start_adc_conversion(void)
{
    ADCSRA |= (1 << ADSC);       // Start the conversion
    while (ADCSRA & (1 << ADSC)) // Wait for the conversion to finish
        ;
}

void print_status()
{
    unsigned short val;

    val = ADC;
    print_dec(val);
}

int main(void)
{
    uart_init();                  // Initialize UART    
    while (1)
    {
        ADMUX &= ~(0b1111 << MUX0); // By default the peripheric listen is the potentiometer
        adc_init(); // Initialize ADC
        start_adc_conversion();
        print_status();
        uart_printstr(", ");

        ADMUX &= ~(0b1111 << MUX0); // By default the peripheric listen is the potentiometer
        ADMUX |= (1 << MUX0);
        adc_init(); // Initialize ADC
        start_adc_conversion();
        print_status();
        uart_printstr(", ");

        ADMUX &= ~(0b1111 << MUX0); // By default the peripheric listen is the potentiometer
        ADMUX |= (1 << MUX1);
        adc_init(); // Initialize ADC
        start_adc_conversion();
        print_status();
        _delay_ms(20);
        uart_printstr("\r\n");
    }
    return 0;
}
#include "../uart.h"

void adc_init(void)
{
    ADMUX &= ~(0b1111 << MUX0); // By default the peripheric listen is the potentiometer
    ADMUX |= (1 << MUX3);
    ADMUX |= (1 << REFS0) | (1 << REFS1);                 // Set voltage reference as 1.1V
    ADMUX &= ~(1 << ADLAR);                               // Set 10bits only precision
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
    int prop = 314 / 25;
    print_dec(val / prop);
}

int main(void)
{
    uart_init(); // Initialize UART
    adc_init();  // Initialize ADC
    while (1)
    {
        start_adc_conversion();
        print_status();
        uart_printstr("\r\n");
        _delay_ms(500);
    }
    return 0;
}
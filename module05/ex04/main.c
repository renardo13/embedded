#include "../uart.h"

void adc_init(void)
{
    ADMUX |= (1 << REFS0);                                // Set AVCC as voltage reference
    ADMUX &= ~(1 << ADLAR);                               // Set 8bits only precision
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler = 128 (appropriate for 16 MHz clock)
    ADCSRA |= (1 << ADEN);                                // Enable ADC
}

void adc_conversion(void)
{
    ADCSRA |= (1 << ADSC);       // Start the conversion
    while (ADCSRA & (1 << ADSC)) // Wait for the conversion to finish
        ;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0B = r; // PD5 (Red)
    OCR0A = g; // PD6 (Green)
    OCR2B = b; // PD3 (Blue)
}

void wheel(uint8_t pos)
{
    pos = 255 - pos;
    if (pos < 85)
        set_rgb(255 - pos * 3, 0, pos * 3);
    else if (pos < 170)
    {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    }
    else
    {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

// Function to initialize Timer0 and Timer2 for PWM
void init_rgb()
{
    /* Timer 0 register A and B */
    TCCR0A |= (1 << COM0B1) | (1 << COM0A1); // For led PD6 and PD5 (non-inverted PWM mode)
    TCCR0A |= (1 << WGM01) | (1 << WGM00);   // Fast PWM mode
    TCCR0B |= (1 << CS02) | (1 << CS00);     // Prescaler of 1024

    /* Timer 2 register A and B */
    TCCR2A |= (1 << COM2B1);                           // For the led PD3 (non-inverted PWM mode)
    TCCR2A |= (1 << WGM21) | (1 << WGM20);             // Fast PWM mode
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler of 1024
}

void handle_led(uint8_t val)
{
    if (val < 255 / 4)
        PORTB = 0;
    if (val >= 255 / 4)
        PORTB = 0b00000001;
    if (val >= 255 / 4 * 2)
        PORTB = 0b00000011;
    if (val >= 255 / 4 * 3)
        PORTB = 0b00000111;
    if (val == 255)
        PORTB = 0b00010111;
}

int main(void)
{
    uart_init(); // Initialize UART
    adc_init();  // Initialize ADC

    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);              // Configure the LED RGB pins in output mode
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4); // Configure the LED RGB pins in output mode

    init_rgb(); // Initialize the PWM timers

    while (1)
    {
        adc_conversion();
        wheel(ADC / 4);
        handle_led(ADC / 4);
    }
    return 0;
}

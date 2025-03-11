#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUDRATE 115200 // Bit rate
#define PRESCALER 256   // Prescaler for Timer1

// Function to transmit a character via UART
void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;     // Wait for transmit buffer to be empty
    UDR0 = c; // Send the character
}

void uart_printstr(const char *str)
{
    for (int i = 0; str[i]; i++)
        uart_tx(str[i]);
}

// Function to initialize UART
void uart_init(void)
{
    unsigned long uart_baudrate;

    UCSR0B = (1 << TXEN0);                         // Enable transmitter
    uart_baudrate = F_CPU / (16 * (BAUDRATE + 1)); // Calculate baud rate register value
    UBRR0H = uart_baudrate >> 8;                   // Set high byte of baud rate
    UBRR0L = uart_baudrate;                        // Set low byte of baud rate
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);        // Set frame format: 8 data bits, no parity, 1 stop bit (8N1)
}

// Function to initialize Timer1
void set_timer()
{
    SREG |= (1 << 7); // Enable global interrupts

    TCCR1B |= (1 << WGM12) | (1 << CS12); /* CTC mode with OCR1A as TOP value + prescaler 256 */
    OCR1A = ((F_CPU / PRESCALER / 1000) * 20);        /* Timer frequency */
    TIMSK1 |= (1 << OCIE1A);              // Enable Timer1 compare interrupt
}

void set_analog_to_digital_conv(void)
{
    ADMUX &= ~(0b1111 << MUX0);
    ADMUX |= (1 << MUX1);
    ADMUX |= (1 << REFS0);                                // Set AVCC as voltage reference
    ADMUX |= (1 << ADLAR);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler = 128 (appropriate for 16 MHz clock)
    ADCSRA |= (1 << ADEN);                                // Enable ADC
}

void start_adc_conversion(void)
{
    ADCSRA |= (1 << ADSC);       // Start the conversion
    while (ADCSRA & (1 << ADSC)); // Wait for the conversion to finish;
}

// ISR for Timer1
ISR(TIMER1_COMPA_vect)
{
    start_adc_conversion();

    uint8_t val = ADCH;

    char base[16] = "0123456789ABCDEF";

    uart_tx(base[val >> 4]);
    uart_tx(base[val & 0x0F]);
    uart_printstr("\b\b");
}

int main(void)
{
    uart_init();                  // Initialize UART
    set_timer();                  // Initialize Timer1
    set_analog_to_digital_conv(); // Initialize ADC

    while (1)
    { 
        
    }
    return 0;
}

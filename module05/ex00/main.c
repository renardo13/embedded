
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

ISR(TIMER1_COMPA_vect)
{
    
    uart_tx(ADCL & 1);
}

// Function to initialize UART
void uart_init(void)
{
    unsigned long uart_baudrate;

    UCSR0B = (1 << TXEN0);                       // Enable transmitter
    uart_baudrate = F_CPU / (16 * (BAUDRATE + 1)); // Calculate baud rate register value
    UBRR0H = uart_baudrate >> 8;                 // Set high byte of baud rate
    UBRR0L = uart_baudrate;                      // Set low byte of baud rate
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set frame format: 8 data bits, no parity, 1 stop bit (8N1)
}

// Function to initialize Timer1
void set_timer()
{
    SREG |= (1 << 7);

    TCCR1B |= (1 << WGM12) | (1 << CS12); /* CTC mode with OCR1A as TOP value + prescaler 256 */
    OCR1A = ((F_CPU / PRESCALER) / 1000) * 20;            /* 1 Hz frequency */
    TIMSK1 |= (1 << OCIE1A);
}

void    set_analog_to_digital_conv(void)
{
    ADMUX |= (1 << REFS0); // Setting AVVC as voltage reference
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE); // ADC Enable; ADSC to start each conversion. ADATE = to configure trigger ADC
    ADCSRB |= (1 << ADTS0); // Analog comparator rtigger source
}

int main(void)
{
    uart_init(); // Initialize UART
    set_timer(); // Initialize Timer1
    set_analog_to_digital_conv(); // Configure ADC
   
    while (1)
    {
        ADCL;
    }
    return 0;
}
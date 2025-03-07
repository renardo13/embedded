
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUDRATE 115200 // Bit rate
#define PRESCALER 1024   // Prescaler for Timer1

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

ISR(TIMER1_COMPA_vect)
{
    uart_printstr("Hello Word!\r\n"); // Transmit 'Z' via UART with cariage return
}

// Function to initialize UART
void uart_init(void)
{
    unsigned int uart_baudrate;

    UCSR0B = (1 << TXEN0);                             // Enable transmitter
    uart_baudrate = F_CPU / (16 * (BAUDRATE) + 1); // Calculate baud rate register value

    UBRR0H = uart_baudrate >> 8; // Set high byte of baud rate
    UBRR0L = uart_baudrate;      // Set low byte of baud rate
}

// Function to initialize Timer1
void set_timer()
{
    SREG |= (1 << 7);

    TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12); /* CTC mode with OCR1A as TOP value + prescaler 1024 */
    OCR1A = (F_CPU / PRESCALER) * 2;            /* 1 Hz frequency */
    TIMSK1 |= (1 << OCIE1A);
}

int main(void)
{
    uart_init(); // Initialize UART
    set_timer(); // Initialize Timer1

    while (1)
    {
    }
    return 0;
}

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

// Function to transmit a character via UART
char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return (UDR0);
}

char reverse_case(char c)
{
    if(c > 'A' && c < 'Z')
        return(c + 32);
    if(c > 'a' && c < 'z')
        return(c - 32);
}

ISR(USART_RX_vect)
{
    uart_tx(reverse_case(uart_rx()));
}

// Function to initialize UART
void uart_init(void)
{
    unsigned long uart_baudrate;

    SREG |= (1 << 7);
    UCSR0B |= (1 << RXCIE0); // X Complete Interrupt Enable n

    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);          // Enable transmitter
    uart_baudrate = F_CPU / (16 * (BAUDRATE + 1)); // Calculate baud rate register value
    UBRR0H = uart_baudrate >> 8;                   // Set high byte of baud rate
    UBRR0L = uart_baudrate;                        // Set low byte of baud rate
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set frame format: 8 data bits, no parity, 1 stop bit (8N1)
}

int main(void)
{
    uart_init(); // Initialize UART

    while (1)
    {
    }
    return 0;
}

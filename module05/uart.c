#include "uart.h"

/***** Library for UART protocol *****/

/* Init ------------------------------------------------------------------------------------------------------------------------------------------------------------------ */

void uart_init(void)
{
    unsigned long uart_baudrate;

    UCSR0B = (1 << TXEN0);                         // Enable transmitter
    uart_baudrate = F_CPU / (16 * (BAUDRATE + 1)); // Calculate baud rate register value
    UBRR0H = uart_baudrate >> 8;                   // Set high byte of baud rate
    UBRR0L = uart_baudrate;                        // Set low byte of baud rate
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);        // Set frame format: 8 data bits, no parity, 1 stop bit (8N1)
}

/* Send and receive ------------------------------------------------------------------------------------------------------------------------------------------------------------------ */

// Function that allow the microcontroler to receive a character from a peripheric
char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return (UDR0);
}

// Function to transmit a character from the microcontroler to the peripheric (pc)
void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;     // Wait for transmit buffer to be empty
    UDR0 = c; // Send the character
}

/* Display ------------------------------------------------------------------------------------------------------------------------------------------------------------------ */

void uart_printstr(const char *str)
{
    for (int i = 0; str[i]; i++)
        uart_tx(str[i]);
}

void print_dec(uint16_t val)
{
    if (val > 9)
        print_dec(val / 10);
    uart_tx(val % 10 + '0');
}

void print_hex(uint8_t val) // For a value store in 8bits
{
    char base[16] = "0123456789ABCDEF";
    // Start form the most significant bits to start printing in the good order
    uart_tx(base[val >> 4]); // To keep the 4 most significant bits
    uart_tx(base[val & 15]); // To keep the less significant 4 bits. (15 = 1111)
}

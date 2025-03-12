
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUDRATE 115200 // Bit rate
#define PRESCALER 256   // Prescaler for Timer1

// Function to initialize UART
void uart_init(void)
{
    unsigned long uart_baudrate;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);                       // Enable transmitter
    uart_baudrate = F_CPU / (16 * (BAUDRATE + 1)); // Calculate baud rate register value
    UBRR0H = uart_baudrate >> 8;                 // Set high byte of baud rate
    UBRR0L = uart_baudrate;                      // Set low byte of baud rate
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set frame format: 8 data bits, no parity, 1 stop bit (8N1)
}

int main(void)
{
    uart_init(); // Initialize UART

    while (1)
    {
        uart_tx(uart_rx());
    }
    return 0;
}
#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUDRATE 115200 // Bit rate
#define PRESCALER 256   // Prescaler for Timer1

void uart_init(void);
char uart_rx(void);
void uart_tx(char c);
void print_dec(uint16_t val);
void uart_printstr(const char *str);
void print_hex(uint8_t val);

#endif
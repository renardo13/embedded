#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUDRATE 115200 // Bit rate
#define PRESCALER 256   // Prescaler for Timer1

int strcmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s2++;
        s1++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

void led_init()
{
    DDRB |= 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4;
}

void led_blink()
{
    for (int i = 0; i < 15; i++)
    {
        _delay_ms(100);
        PORTB ^= 1 << PB0;
        _delay_ms(100);
        PORTB ^= 1 << PB1;
        _delay_ms(100);
        PORTB ^= 1 << PB2;
        _delay_ms(100);
        PORTB ^= 1 << PB4;
    }
}

int main(void)
{
    char c;
    int i;
    char username[255];
    char password[255];

    c = 0;
    led_init();
    uart_init(); // Initialize UART protocol
    while (1)
    {
        uart_printstr("Enter your login:\r\n");
        uart_printstr("\tUsername: ");
        i = 0;
        c = 0;
        while (c != '\r')
        {
            c = uart_rx();
            if (c == 127 && i > 0)
            {
                username[--i] = '\0';
                uart_printstr("\b \b");
            }
            else if (c != '\r' && c != 127)
                username[i++] = c;
            uart_tx(c);
        }
        username[i] = '\0';
        uart_printstr("\r\n\tPassword: ");
        i = 0;
        c = 0;
        while (c != '\r')
        {
            c = uart_rx();
            if (c == 127 && i > 0)
            {
                password[--i] = '\0';
                uart_printstr("\b \b");
            }
            else if (c != '\r' && c != 127)
            {
                password[i++] = c;
                c = '*';
            }
            uart_tx(c);
        }
        password[i] = '\0';
        if (!strcmp("melmarti", username) && !strcmp("123", password))
        {
            uart_printstr("\r\nHello ");
            uart_printstr(username);
            uart_printstr("!\r\n\n");
            led_blink();
        }
        else
            uart_printstr("\r\nBad combination username/password\r\n\n");
    }
    return (0);
}
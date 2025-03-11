#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* There are 3 things to parameters for timers. The mode of the pins related to what pin we want to control
(it is not obligatory) the waveform(WGMx) -> the way the timer counts.
And finally the prescaler(the proportional divisor, which allows the timer to count less for one cycle) */

#define BAUDRATE 115200 // Bit rate
#define PRESCALER 1024

/* RED -> PD5
GREEN -> PD6
BLUE -> PD3 */

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;     // Wait for transmit buffer to be empty
    UDR0 = c; // Send the character
}

char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return (UDR0);
}

void uart_printstr(char *s)
{
    int i;

    i = 0;
    while (s[i])
        uart_tx(s[i++]);
}

// Function to initialize UART
void uart_init(void)
{
    unsigned long uart_baudrate;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);          // Enable transmitter
    uart_baudrate = F_CPU / (16 * (BAUDRATE + 1)); // Calculate baud rate register value
    UBRR0H = uart_baudrate >> 8;                   // Set high byte of baud rate
    UBRR0L = uart_baudrate;                        // Set low byte of baud rate
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);        // Set frame format: 8 data bits, no parity, 1 stop bit (8N1)
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0B = r; // PD5 (Red)
    OCR0A = g; // PD6 (Green)
    OCR2B = b; // PD3 (Blue)
}

uint8_t char_to_hex(char s[2])
{
    uint8_t nb;

    if (s[0] >= '0' && s[0] <= 'F')
        nb = (s[0] - 48) * 16;
    if (s[1] >= '0' && s[1] <= 'F')
        return (nb + (s[1] - 48));
    uart_printstr("Not hexa value inserted\r\n");
    return (-1);
}

void parse_color(char *buff)
{
    if (buff[0] == '#')
    {
        uint8_t red = char_to_hex(buff + 1);
        uint8_t green = char_to_hex(buff + 3);
        uint8_t blue = char_to_hex(buff + 5);
        set_rgb(red, green, blue);
    }
    else
        uart_printstr("\r\nWrong format, please start with #");
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

int is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

int main(void)
{
    char buffer[255];
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6); // Configure the LED RGB pins in output mode
    init_rgb();  // Initialize timer to control the intensity of RGB colors
    uart_init(); // Initialize UART
    while (1)
    {
        for (int i = 0; i < 255; i++)
        {
            char c = uart_rx();
            if (is_printable(c))
            {
                buffer[i] = c;
                uart_tx(c);
            }
            else if (c == 127 && i > 0)
            {
                i--;
                buffer[i] = '\0';
                uart_printstr("\b \b");
                i--;
            }
            else if (c == '\r')
            {
                buffer[i] = '\0';
                if (i == 7)
                    parse_color(buffer);
                else
                    uart_printstr("\r\nWrong format, please enter a color at RGB format #RRGGBB");
                uart_printstr("\r\n");
                break;
            }
        }
    }
}
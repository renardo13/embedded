#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define PRESCALER 1024

/* RED -> PD5
GREEN -> PD6
BLUE -> PD3 */

ISR(TIMER1_COMPA_vect)
{
    static int i = 0;
    static uint8_t red[7] = {1, 0, 0, 1, 0, 1, 1};
    static uint8_t green[7] = {0, 1, 0, 1, 1, 0, 1};
    static uint8_t blue[7] = {0, 0, 1, 0, 1, 1, 1};

    PORTD = (red[i] << PD5) | (green[i] << PD6) | (blue[i] << PD3);
    i++;
    if(i == 7)
        i = 0;
}

// Function to initialize Timer1
void set_timer()
{
    SREG |= (1 << 7);
    TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12); /* CTC mode with OCR1A as TOP value + prescaler 1024 */
    OCR1A = (F_CPU / PRESCALER);                        /* 1 Hz frequency */
    TIMSK1 |= (1 << OCIE1A);                            /* Activation of the interruption when the compare value matches */
}

int main(void)
{
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6); // Configure the LED RGB in output mode

    set_timer();
    while (1)
    {
    }
}
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* There are 3 things to parameters for timers. The mode of the pins related to what pin we want to control
(it is not obligatory) the waveform(WGMx) -> the way the timer counts. 
And finally the prescaler(the proportional divisor, which allows the timer to count less for one cycle) */

#define PRESCALER 1024

/* RED -> PD5
GREEN -> PD6
BLUE -> PD3 */

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0B = r;  // PD5 (Red)
    OCR0A = g;  // PD6 (Green)
    OCR2B = b;  // PD3 (Blue)
}

void wheel(uint8_t pos)
{
    pos = 255 - pos;
    if (pos < 85)
        set_rgb(255 - pos * 3, 0, pos * 3);
    else if (pos < 170)
    {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    }
    else
    {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

// Function to initialize Timer0 and Timer2 for PWM
void init_rgb()
{
    /* Timer 0 register A and B */
    TCCR0A |= (1 << COM0B1) | (1 << COM0A1); // For led PD6 and PD5 (non-inverted PWM mode)
    TCCR0A |= (1 << WGM01) | (1 << WGM00); // Fast PWM mode
    TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler of 1024

    /* Timer 2 register A and B */
    TCCR2A |= (1 << COM2B1); // For the led PD3 (non-inverted PWM mode)
    TCCR2A |= (1 << WGM21) | (1 << WGM20); // Fast PWM mode
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler of 1024
}

int main(void)
{
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6); // Configure the LED RGB pins in output mode

    init_rgb(); // Initialize the PWM timers

    while (1)
    {
        for (int pos = 0; pos < 255; pos++)
        {
            wheel(pos);
            _delay_ms(20);
        }
        // Optionally, to create a smooth color cycle, you could reverse the direction:
        for (int pos = 255; pos >= 0; pos--)
        {
            wheel(pos);
            _delay_ms(20);
        }
    }
}
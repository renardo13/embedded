#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL // Définir la fréquence de l'horloge du microcontrôleur
#define PRESCALER 1024

uint8_t is_pressed = 0;
uint8_t value = 0;

ISR(TIMER0_COMPA_vect)
{
	while(!(1 & PD4)) {}
	if (value < 15)
		value++;
	_delay_ms(10);
	PORTB = (value & 8) << 1 | (value & 7);
}

ISR(INT0_vect)
{
	is_pressed = !is_pressed;

	if (is_pressed && value > 0)
		value++;
	_delay_ms(10);
	PORTB = (value & 8) << 1 | (value & 7);
}

void set_timer()
{
	// Interrupt for SW1
	SREG |= (1 << 7);
	EIMSK |= (1 << INT0); // Enable interrutpion
	EICRA |= (1 << ISC11); // Interruption request mode logical change for INT0 (SW1) and INT1 (SW2)
	TIMSK0 |= (1 << OCIE0A); 

	// // Configurer Timer0 pour SW2
	// TCCR0A |= (1 << WGM01);  // Mode CTC (Clear Timer on Compare Match)
	// TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler de 1024
	// OCR0A = (F_CPU / PRESCALER) / 1000; // Fréquence de 1kHz (interruption toutes les 1ms)
}

int main(void)
{
	// Configurer PB1 comme sortie pour OC1A
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

	set_timer();

	while (1){}
	return 0;
}

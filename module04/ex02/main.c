#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL // Définir la fréquence de l'horloge du microcontrôleur
#define PRESCALER 1024

uint8_t is_pressed = 0;
int value = 0;


ISR(PCINT2_vect)
{
	is_pressed = !is_pressed;

	if (is_pressed && value > 0)
		value--;
	_delay_ms(10);
	PORTB = (value & 8) << 1 | (value & 7);
}


ISR(INT0_vect)
{
	is_pressed = !is_pressed;

	if (is_pressed && value < 15)
		value++;
	_delay_ms(10);
	PORTB = (value & 8) << 1 | (value & 7);
}


void set_timer()
{
	SREG |= (1 << 7);
	EIMSK |= (1 << INT0); // Enable interrutpion for SW1
	EICRA |= (1 << ISC00); // Interruption request mode logical change for INT0 (SW1)

	PCMSK2 |= (1 << PCINT20); // Enable interrutpion for SW2
	PCICR |= (1 << PCIE2);
}

int main(void)
{
	// Configurer PB1 comme sortie pour OC1A
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

	set_timer();

	while (1)
	{

		// Les interruptions ajustent automatiquement le rapport cyclique de la LED
	}

	return 0;
}

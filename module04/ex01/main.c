#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL // Définir la fréquence de l'horloge du microcontrôleur
#define PRESCALER 1024

int proportion = 1; // Valeur de proportion pour augmenter ou diminuer le rapport cyclique

// Interruption pour Timer0
ISR(TIMER0_COMPA_vect)
{
	if (OCR1A >= 255)
		proportion = -1; // Inverser la direction lorsque le rapport cyclique atteint 100%
	else if (OCR1A <= 0)
		proportion = 1; // Inverser la direction lorsque le rapport cyclique atteint 0%
	OCR1A += proportion; // Ajuster le rapport cyclique de la LED en fonction de la proportion
}

void set_timer()
{
	sei(); // Activer les interruptions globales

	// Configuration de Timer1 pour le mode Fast PWM
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1); // Non inversé sur OC1A et OC1B
	TCCR1A |= (1 << WGM10) | (1 << WGM11);	 // Mode Fast PWM pour Timer1
	TCCR1B |= (1 << WGM12) | (1 << CS12);	 // Prescaler de 256 pour Timer1

	// Configuration de Timer0 pour générer des interruptions toutes les 1 ms (1000 Hz)
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);			// Non inversé sur OC0A et OC0B
	TCCR0A |= (1 << WGM00) | (1 << WGM01);				// Mode Fast PWM pour Timer0
	TCCR0B |= (1 << WGM02) | (1 << CS02) | (1 << CS00); // Prescaler de 1024 pour Timer0

	// Enable interruption pour Timer0
	TIMSK0 |= (1 << OCIE0A); // Interruption à chaque correspondance de OCR0A

	// Définir OCR0A pour une interruption toutes les 1 ms (1000 Hz)
	OCR0A = (F_CPU / PRESCALER) / 1000; // Fréquence d'interruption de 1000 Hz (toutes les 1 ms)

	// Initialiser OCR1A à 0 pour un rapport cyclique initial de 0%
	OCR1A = 0;
}

int main(void)
{
	// Configurer PB1 comme sortie pour OC1A
	DDRB |= (1 << PB1);

	// Initialiser les timers
	set_timer();

	// Boucle infinie
	while (1)
	{
		// Les interruptions ajustent automatiquement le rapport cyclique de la LED
	}

	return 0;
}

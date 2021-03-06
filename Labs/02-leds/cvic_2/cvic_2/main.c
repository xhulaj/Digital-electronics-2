	/***********************************************************************
	 * 
	 * Alternately toggle two LEDs when a push button is pressed.
	 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
	 *
	 * Inspired by Tomas Fryza
	 * Dept. of Radio Electronics, Brno University of Technology, Czechia
	 * This work is licensed under the terms of the MIT license.
	 * 
	 **********************************************************************/

	/* Defines -----------------------------------------------------------*/
	#define LED_GREEN   PB5     // AVR pin where green LED is connected
	#define LED_RED     PC0
	#define BTN			PD0
	#define BLINK_DELAY 250
	#ifndef F_CPU
	#define F_CPU 16000000      // CPU frequency in Hz required for delay
	#endif

	/* Includes ----------------------------------------------------------*/
	#include <util/delay.h>     // Functions for busy-wait delay loops
	#include <avr/io.h>         // AVR device-specific IO definitions

	/* Functions ---------------------------------------------------------*/
	/**
	 * Main function where the program execution begins. Toggle two LEDs 
	 * when a push button is pressed.
	 */
	int main(void)
	{
		/* GREEN LED */
		// Set pin as output in Data Direction Register...
		DDRB = DDRB | (1<<LED_GREEN);
		// ...and turn LED off in Data Register
		PORTB = PORTB & ~(1<<LED_GREEN);

		/* second LED */
		// WRITE YOUR CODE HERE
		DDRC = DDRC | (1<<LED_RED);
		PORTC = PORTC & ~(1<<LED_RED);
		/* button w pullup resistor */
		DDRD = DDRD & ~(1<<BTN);
		PORTD = PORTD | (1<<BTN);
    
	
		while (1)
		{
			// Pause several milliseconds
		   // _delay_ms(BLINK_DELAY);
			if (bit_is_clear(PIND, BTN))
			{
			_delay_ms(BLINK_DELAY);
			PORTC = PORTC ^ (1<<LED_RED);
			PORTB = PORTB ^ (1<<LED_GREEN);
			}
		
			while(bit_is_set(PIND, BTN))
			{
				PORTC = PORTC & ~(0<<LED_RED);
				PORTB = PORTB & ~(0<<LED_GREEN);
			}
		}
	
    
		return 0;
	}
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
	#define BIT(x) (0x01 << (x))
	#define bit_get(p,m) ((p) & (m))
	#define bit_set(p,m) ((p) |= (m))
	#define bit_clear(p,m) ((p) &= ~(m))
	#define LED0   PB0     // AVR pin where green LED is connected
	#define LED1   PB1
	#define LED2   PB2
	#define LED3   PB3
	#define LED4   PB4
	#define LED5   PB5
	#define LED6   PB6
	#define LED7   PB7
	//#define LED_CHCK  PB0
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
		/* Set direction off all pins connected to LEDs */
		DDRB = 0b11111111;
		//turn LEDs off in Data Register
		PORTB = 0b00000000;
		
		/* button w pullup resistor */
		DDRD = DDRD & ~(1<<BTN);
		PORTD = PORTD | (1<<BTN);
		
		//  set a variable for button check
		int8_t BTN_CHCK = 0b0000;
		int8_t n = 0x00;
	
		while (1)
		{
			
			_delay_ms(BLINK_DELAY);
			
			// flip LSb in variable BTN_CHCK if button is pressed
			if(~(bit_get(PORTD, BIT(0))))
			{

				BTN_CHCK ^= 0x01;
				PORTB = 0b11111111;
			}
			// if BTN_CHCK has a nonzero value, start K.I.T.T
			if(bit_get(BTN_CHCK, BIT(0)))
			{
				for(n = 0; n <= 7; n++)
				{
				bit_clear(PORTB, BIT(n));
				_delay_ms(50);
				bit_set(PORTB, BIT(n-1));
				_delay_ms(BLINK_DELAY);
				}
				for(n = 7; n >=0; n--)
				{
					bit_clear(PORTB, BIT(n));
					_delay_ms(50);
					bit_set(PORTB, BIT(n+1));
					_delay_ms(BLINK_DELAY);
				}
				
			}
		}
    
		return 0;
	}
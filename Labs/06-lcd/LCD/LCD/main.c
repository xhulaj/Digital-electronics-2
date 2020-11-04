/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows.
 */
uint8_t customChar[] = {
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110
};
int main(void)
{
	 // Initialize LCD display
	 lcd_init(LCD_DISP_ON);

	 // Set pointer to beginning of CGRAM memory
	 lcd_command(1 << LCD_CGRAM);
	 for (uint8_t i = 0; i < 32; i++)
	 {
		 // Store all new chars to memory line by line
		 lcd_data(customChar[i]);
	 }
	 // Set DDRAM address
	 lcd_command(1 << LCD_DDRAM);
	 
	 // Display first custom character
    // Initialize LCD display
    //lcd_init(LCD_DISP_ON);

    // Put string(s) at LCD display
    lcd_gotoxy(1, 0);
    lcd_puts("00:00.0");
	lcd_gotoxy(1,1);
	lcd_putc('b');
	lcd_gotoxy(11,1);
	lcd_putc('c');

    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Set prescaler and enable overflow interrupt every 16 ms
	TIM2_overflow_16ms();
	TIM2_overflow_interrupt_enable();
	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
	static uint8_t tens = 0;
	static uint8_t secs = 00;
	static uint8_t mins = 0;
	uint16_t sq_mins = secs * secs;	
	
	char LCD_string[2] = "  ";
	char sq_string[4] = "    ";

    number_of_overflows++;
    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;
		tens++;
		if(tens > 9)
		{
			tens = 0;
			secs++;
		}
		if(secs > 59)
		{
			secs = 0;
			lcd_gotoxy(11,0);
			lcd_putc(32);
			lcd_putc(32);
			lcd_putc(32);
			lcd_putc(32);
			sq_mins = 0;
			mins++;
		}
		if(mins > 59)
		{
			mins = 0;
		}

		itoa(tens, LCD_string, 10);
		lcd_gotoxy(7,0);
		lcd_puts(LCD_string);
		if(secs < 10)
		{
			itoa(secs, &LCD_string[1],10);
			LCD_string[0] = '0';
		}
		if(secs > 9)
		{
			itoa(secs, &LCD_string[0],10);
		}
		lcd_gotoxy(4, 0);
		lcd_puts(LCD_string);
		if(mins < 10)
		{
			itoa(mins, &LCD_string[1],10);
			LCD_string[0] = '0';
		}
		if(mins > 9)
		{
			itoa(mins, &LCD_string[0],10);
		}
		lcd_gotoxy(1, 0);
		lcd_puts(LCD_string);
		// print square of mins to screen
		itoa(sq_mins, sq_string, 10);
		lcd_gotoxy(11,0);
		lcd_puts(sq_string);
    }

}
ISR(TIMER0_OVF_vect)
{
	static uint8_t bar = 0;
	static uint8_t pos = 0;
	static uint8_t symbol[6] = {32, 0, 1, 2, 3, 255};
	lcd_gotoxy(pos, 1);
	lcd_putc(symbol[bar]);
	bar++;
	
	if(bar > 5)
	{
		pos++;
		bar = 0;
	}
	if( pos > 9)
	{
		pos = 0;
		bar = 0;
		lcd_gotoxy(0,1);
		for(int i = 0; i < 10; i++)
		{
			lcd_putc(32);
		}
	}
}
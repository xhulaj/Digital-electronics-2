/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
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
#include "uart.h"           // Peter Fleury's UART library
#ifndef F_CPU
#define F_CPU 16000000
#endif

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */
	int8_t func_parity( uint16_t n)
	{
		uint8_t parity = 0;
		while(n)
		{
			parity = !parity;
			n = n & (n-1);
		}
		return parity;
	}
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");    // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");    // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");    // Put button name here

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
    // Set input channel to ADC0
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    // Enable ADC module
	ADCSRA |= (1 << ADEN);
    // Enable conversion complete interrupt
	ADCSRA |= (1 << ADIE);
    // Set clock prescaler to 128
	ADCSRA |= (1 << ADPS2);
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS0);
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();
    // Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600,F_CPU));

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
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
    ADCSRA |= (1 << ADSC);

}

/* -------------------------------------------------------------------*/
/**
 * ISR starts when ADC completes the conversion. Display value on LCD
 * and send it to UART.
 */
	ISR(ADC_vect)
	{
		 static char buttons[5][7] = 
		{
			"RIGHT ",
			"UP    ",
			"LEFT  ",
			"DOWN  ",
			"SELECT"
		};
		static char current_button[7];
		uint16_t value = 0;
		char lcd_string[4] = "    ";
		value = ADC;    // Copy ADC result to 16-bit variable
		lcd_gotoxy(7, 0);
		lcd_puts(lcd_string);
		itoa(value, lcd_string, 10);    // Convert to string in decimal
		lcd_gotoxy(7, 0);
		lcd_puts(lcd_string);
		lcd_gotoxy(12, 0);
		lcd_puts("   ");
		lcd_gotoxy(12, 0);
		itoa(value, lcd_string, 16);
		lcd_puts(lcd_string);
		if(value < 50)
		{
			for(int i = 0; i <= 7; i++)
			{
			current_button[i] = buttons[0][i];
			}
		}
		else if((value > 50) && (value < 180))
		{
			for(int i = 0; i <= 7; i++)
			{
				current_button[i] = buttons[1][i];
			}
		}
		else if((value > 180) && (value < 300))
		{
			for(int i = 0; i <= 7; i++)
			{
				current_button[i] = buttons[2][i];
			}
		}
		else if((value > 300) && (value < 500))
		{
			for(int i = 0; i <= 7; i++)
			{
				current_button[i] = buttons[3][i];
			}
		}
		else if((value > 500) && (value < 800))
		{
			for(int i = 0; i <= 7; i++)
			{
				current_button[i] = buttons[4][i];
			}
		}
		else
		{
			for(int i = 0; i <= 7; i++)
			{
				current_button[i] = ' ';
			}
		}
		uint16_t Voltage = value * 4.88;
		lcd_gotoxy(8, 1);
		lcd_puts(current_button);
		if(value < 1010)
		{
			uart_puts(current_button);
			uart_puts(" : ");
			itoa(value, lcd_string, 10);
			uart_puts(lcd_string);
			uart_puts(" @ ");
			itoa(Voltage, lcd_string, 10);
			uart_puts(lcd_string);
			uart_puts(" mV");
			uart_puts(" Parity: ");
			uint8_t parity = func_parity(value);
			lcd_gotoxy(13, 1);
			itoa(parity, lcd_string, 2);
			lcd_puts(lcd_string);
			uart_puts(lcd_string);
			uart_puts("\r\n");
		}
	}
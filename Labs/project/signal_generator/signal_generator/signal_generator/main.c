/*
 * signal_generator.c
 *
 * Created: 19.11.2020 19:20:24
 * Author : Hula Josef (212557), Elias Martin (...)
 */ 
/*  Defines -------------------------------------------------------------------------- */
#define KEYBOARD_ROW_0		PD0		// AVR pin where first row of keyboard is connected
#define KEYBOARD_ROW_1		PD1		// AVR pin where second row of keyboard is connected
#define KEYBOARD_ROW_2		PD2		// AVR pin where third row of keyboard is connected
#define KEYBOARD_ROW_3		PD3		// AVR pin where fourth row of keyboard is connected
#define KEYBOARD_COL_0		PD4		// AVR pin where first column of keyboard is connected
#define KEYBOARD_COL_1		PD5		// AVR pin where second column of keyboard is connected
#define KEYBOARD_COL_2		PD6		// AVR pin where third column of keyboard is connected

#ifndef F_CPU
#define F_CPU			16000000	// CPU frequency in Hz 
#endif


#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "lcd.h"

int main(void)
{
	/* Set pins 0 - 3 on port D to output with low, 4 - 6 to input with pullup resistor */
	GPIO_config_output(&DDRD, KEYBOARD_ROW_0);
	GPIO_write_low(&DDRD, KEYBOARD_ROW_0);
	GPIO_config_output(&DDRD, KEYBOARD_ROW_1);
	GPIO_write_low(&DDRD, KEYBOARD_ROW_1);
	GPIO_config_output(&DDRD, KEYBOARD_ROW_2);
	GPIO_write_low(&DDRD, KEYBOARD_ROW_2);
	GPIO_config_output(&DDRD, KEYBOARD_ROW_3);
	GPIO_write_low(&DDRD, KEYBOARD_ROW_3);
	GPIO_config_input_pullup(&DDRD, KEYBOARD_COL_0);
	GPIO_config_input_pullup(&DDRD, KEYBOARD_COL_1);
	GPIO_config_input_pullup(&DDRD, KEYBOARD_COL_2);
    /* Replace with your application code */
    while (1) 
    {
    }
}


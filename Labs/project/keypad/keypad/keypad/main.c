

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <stdlib.h>         // C library. Needed for conversion function

#include "gpio.h"          // Gpio library
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library

#include "uart.h"           // Peter Fleury's UART library

#include "debounce.h"




#ifndef F_CPU
#define F_CPU 16000000
#endif


int main(void)
{
	//LCD settings
	lcd_init(LCD_DISP_ON);
	lcd_gotoxy(1, 0); lcd_puts("            ");
	lcd_gotoxy(1, 1); lcd_puts("            ");
	
	/*
	GPIO_config_input_nopull(&DDRC, PC2);	
	
	GPIO_config_output(&DDRC, PC1);
	GPIO_write_high(&PORTC, PC1);
	*/
	
	//##########################################################ADC settings
    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);
    
    // Set input channel to ADC0
    ADMUX &= ~((1 << MUX0)|(1 << MUX1)|(1 << MUX2)|(1 << MUX3));

    // Enable ADC module
    ADCSRA |= (1 << ADEN);

    // Enable conversion complete interrupt
    ADCSRA |= (1 << ADIE);

    // Set clock prescaler to 128
    ADCSRA |= ((1 << ADPS0)|(1 << ADPS1)|(1 << ADPS2));
	//##########################################################ADC settings
	
    //timer for checking state of buttons every 4ms 
	TIM1_overflow_4ms(); //1s for debugging
	TIM1_overflow_interrupt_enable();
	
	// Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600,F_CPU));

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
		
    }

    // Will never reach this
    return 0;
}

ISR(TIMER1_OVF_vect)
{	
	// Start ADC conversion
	ADCSRA |= (1 << ADSC);
	
	
	
	
	
/*
	static uint8_t button1_history = 0b1111111111111111; //button states history
	static uint8_t count = 0;
	char lcd_string[8] = "        ";
	
	button1_history = (button1_history << 1 ); //make a space for new state
	button1_history |= (GPIO_read(&PINC, PC2) >> PC2); //add new state
	
	if (is_button_pressed(&button1_history))
	{
		count++;
		itoa(count, lcd_string, 10);
		lcd_gotoxy(count, 1);
		lcd_puts(lcd_string);
	}	   

	uart_puts("\r\n");
	itoa(button1_history, lcd_string, 10);	
	uart_puts(lcd_string);
	*/
	
}

ISR(ADC_vect)
{
	//clear decimal and hex position
	uint16_t value = ADC;
		
	//button states history (all of them has not yet been pushed) 
	static uint16_t button_history[16] = { 
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,
		0xFFFF,0xFFFF,0xFFFF,0xFFFF		
	};
	
	for(uint8_t i = 0; i < 16; ++i) //for all buttons
	{
	button_history[i] = (button_history[i] << 1); //make a space for new state
	button_history[i] |= 1; //set current state as not pushed in button history
	}
			
    //################################## rozpoznani tlacitek
	if (value > 720)
	{				
		//uart_puts("NONE");		
	}
	//       from ADC value          to value - (not included)
	else if (720 >= value && value > 660) //1 - key name
	{		
		button_history[0] &= ~(1<<0); //set current state as pushed in button history
		if (is_button_pressed(&button_history[0])) //button is pressed if it is not bounce or glitch
		{
			uart_puts("1"); //send pushed key on UART		
		}		
	}
	else if (660 >= value && value > 610) //4
	{
		button_history[1] &= ~(1<<0); 
		if (is_button_pressed(&button_history[1]))
		{
			uart_puts("4");		
		}		
	}
	else if (610 >= value && value > 570) //7
	{
		button_history[2] &= ~(1<<0); 
		if (is_button_pressed(&button_history[2]))
		{
			uart_puts("7");
		}
	}
	else if (570 >= value && value > 530) //*
	{
		button_history[3] &= ~(1<<0); 
		if (is_button_pressed(&button_history[3]))
		{
			uart_puts("*");
		}
	}
	else if (530 >= value && value > 490) //2
	{
		button_history[4] &= ~(1<<0); 
		if (is_button_pressed(&button_history[4]))
		{
			uart_puts("2");
		}
	}
	else if (490 >= value && value > 460) //3
	{
		button_history[5] &= 0xFFFE; 
		if (is_button_pressed(&button_history[5]))
		{
			uart_puts("3");
		}
	}
	else if (460 >= value && value > 430) //A
	{
		button_history[6] &= ~(1<<0); 
		if (is_button_pressed(&button_history[6]))
		{
			uart_puts("A");
		}
	}
    else if (430 >= value && value > 390) //5
    {
		button_history[7] &= ~(1<<0); 
		if (is_button_pressed(&button_history[7]))
		{
			uart_puts("5");
		}
    }
	else if (390 >= value && value > 340) //6
	{
		button_history[8] &= ~(1<<0); 
		if (is_button_pressed(&button_history[8]))
		{
			uart_puts("6");
		}
	}
	else if (340 >= value && value > 290) //B
	{
		button_history[9] &= ~(1<<0); 
		if (is_button_pressed(&button_history[9]))
		{
			uart_puts("B");
		}
	}
	else if (290 >= value && value > 230) //8
	{
		button_history[10] &= ~(1<<0); 
		if (is_button_pressed(&button_history[10]))
		{
			uart_puts("8");
		}
	}
	else if (230 >= value && value > 190) //9
	{
		button_history[11] &= ~(1<<0); 
		if (is_button_pressed(&button_history[11]))
		{
			uart_puts("9");
		}
	}
	else if (190 >= value && value > 150) //0
	{
		button_history[12] &= ~(1<<0); 
		if (is_button_pressed(&button_history[12]))
		{
			uart_puts("0");
		}
	}
	else if (150 >= value && value > 110) //C
	{
		button_history[13] &= ~(1<<0); 
		if (is_button_pressed(&button_history[13]))
		{
			uart_puts("C");
		}
	}
	else if (110 >= value && value > 50) //#
	{
		button_history[14] &= ~(1<<0); 
		if (is_button_pressed(&button_history[14]))
		{
			uart_puts("#");
		}
	}
	else if (50 >= value && value >= 0) //D
	{
		button_history[15] &= ~(1<<0); 
		if (is_button_pressed(&button_history[15]))
		{
			uart_puts("D");
		}
	}	
	else
	{		
	}	
    //##################################
			
	
}
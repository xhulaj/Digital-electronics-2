/***********************************************************************
 * 
 * Seven-segment display library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

#define F_CPU 16000000


/* Includes ----------------------------------------------------------*/
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"
//Snake segments
uint8_t snake[] =
{
	0b01111111, // 1st step - A
	0b00111111, // 2nd step - A and B
	0b10111111, // 3 - B
	0b10011111, // 4 - B & C
	0b11011111, // 5 - C
	0b11001111, // 6 - C & D
	0b11101111, // 7 - D
	0b11100111, // 8 - D & E
	0b11110111, // 9 - E
	0b11110011, // 10 - E & F
	0b11111011, // 11 - F
	0b01111011 // 12 - F & A
	
};
//Active-low digits 0 to 9
uint8_t segment_value[] =
{
	//abcdefgDP
	0b00000011, // 0
	0b10011111, // 1
	0b00100101, // 2
	0b00001101, // 3
	0b10011001, // 4
	0b01001001, // 5
	0b01000001, // 6
	0b00011111, // 7
	0b00000001, // 8
	0b00001001  // 9
};

// Active-high position 0 to 3
uint8_t segment_position[] =
{
	// p3 p2 p1 p0
	0b00010000,   // position 0
	0b00100000,   // position 1
	0b01000000,   // position 2
	0b10000000   // position 3
};
/* Function definitions ----------------------------------------------*/
void SEG_init(void)
{
    /* Configuration of SSD signals */
    GPIO_config_output(&DDRD, SEGMENT_LATCH);
    GPIO_config_output(&DDRD, SEGMENT_CLK);
    GPIO_config_output(&DDRB, SEGMENT_DATA);
}

/*--------------------------------------------------------------------*/
void SEG_update_shift_regs(uint8_t segments, uint8_t position)
{
    uint8_t bit_number;
	segments = segment_value[segments];     // 0, 1, ..., 9
	position = segment_position[position];  // 0, 1, 2, 3

    // Pull LATCH, CLK, and DATA low
	
	GPIO_write_low(&PORTD, SEGMENT_LATCH);
	GPIO_write_low(&PORTD, SEGMENT_CLK);
	GPIO_write_low(&PORTB, SEGMENT_DATA);

    // Wait 1 us
	_delay_us(1);
	
    // Loop through the 1st byte (segments)
    // a b c d e f g DP (active low values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Output DATA value (bit 0 of "segments")
		if ((segments % 2) == 0)
		{
			GPIO_write_low(&PORTB, SEGMENT_DATA);
		}
		else
		{
			GPIO_write_high(&PORTB, SEGMENT_DATA);
		}
        // Wait 1 us
		_delay_us(1);
        // Pull CLK high
		GPIO_write_high(&PORTD, SEGMENT_CLK);

        // Wait 1 us
		_delay_us(1);
        // Pull CLK low
		GPIO_write_low(&PORTD, SEGMENT_CLK);
        // Shift "segments"
        segments = segments >> 1;
    }

    // Loop through the 2nd byte (position)
    // p3 p2 p1 p0 . . . . (active high values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Output DATA value (bit 0 of "position")
		if ((position & 1) == 0)
		{
			GPIO_write_low(&PORTB, SEGMENT_DATA);
		}
		else
		{
			GPIO_write_high(&PORTB, SEGMENT_DATA);
		}
        // Wait 1 us
	_delay_us(1);
        // Pull CLK high
		GPIO_write_high(&PORTD, SEGMENT_CLK);
        // Wait 1 us
	_delay_us(1);
        // Pull CLK low
		GPIO_write_low(&PORTD, SEGMENT_CLK);
        // Shift "position"
        position = position >> 1;
    }

    // Pull LATCH high
	GPIO_write_high(&PORTD, SEGMENT_LATCH);
    // Wait 1 us
	_delay_us(1);
}

/*--------------------------------------------------------------------*/
/* SEG_clear */
void SEG_clear(void)
{
	 uint8_t bit_number = 0;
	 uint8_t segments = 0b11111111;
	 uint8_t position = 0;

	 // Pull LATCH, CLK, and DATA low
	 
	 GPIO_write_low(&PORTD, SEGMENT_LATCH);
	 GPIO_write_low(&PORTD, SEGMENT_CLK);
	 GPIO_write_low(&PORTB, SEGMENT_DATA);

	 // Wait 1 us
	 _delay_us(1);
	 
	 // Loop through the 1st byte (segments)
	 // a b c d e f g DP (active low values)
	 for (bit_number = 0; bit_number < 8; bit_number++)
	 {
		 // turn segment off
			 GPIO_write_high(&PORTB, SEGMENT_DATA);
		
		 // Wait 1 us
		 _delay_us(1);
		 // Pull CLK high
		 GPIO_write_high(&PORTD, SEGMENT_CLK);

		 // Wait 1 us
		 _delay_us(1);
		 // Pull CLK low
		 GPIO_write_low(&PORTD, SEGMENT_CLK);
		 // Shift "segments"
		 segments = segments >> 1;
		 
		     // Loop through the 2nd byte (position)
		     // p3 p2 p1 p0 . . . . (active high values)
		     for (bit_number = 0; bit_number < 8; bit_number++)
		     {
			     // Output DATA value (bit 0 of "position")
			     if ((position & 1) == 0)
			     {
				     GPIO_write_low(&PORTB, SEGMENT_DATA);
			     }
			     else
			     {
				     GPIO_write_high(&PORTB, SEGMENT_DATA);
			     }
			     // Wait 1 us
			     _delay_us(1);
			     // Pull CLK high
			     GPIO_write_high(&PORTD, SEGMENT_CLK);
			     // Wait 1 us
			     _delay_us(1);
			     // Pull CLK low
			     GPIO_write_low(&PORTD, SEGMENT_CLK);
			     // Shift "position"
			     position = position >> 1;
		     }

		     // Pull LATCH high
		     GPIO_write_high(&PORTD, SEGMENT_LATCH);
		     // Wait 1 us
		     _delay_us(1);
	     
}
/*--------------------------------------------------------------------*/
/* SEG_clk_2us */
void SEG_clk_2us(void)
{
	//wait 1 us
	_delay_us(1);
	
	//pull CLK high
	GPIO_write_high(&PORTD, SEGMENT_CLK);
	
	//wait 1 us
	_delay_us(1);
	
	//pull CLK low
	GPIO_write_low(&PORTD, SEGMENT_CLK);
}
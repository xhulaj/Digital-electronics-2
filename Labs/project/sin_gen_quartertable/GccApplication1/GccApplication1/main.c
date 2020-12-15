/*
 * GccApplication1.c
 *
 * Created: 15.12.2020 16:33:53
 * Author : spravce
 */ 

/* Defines -----------------------------------------------------------------------*/
#ifndef F_CPU
#define F_CPU			16000000	// CPU frequency in Hz
#endif
#define COMP_REG_A_MASK 0b10100000;

#define R0	PB0
#define R1	PB1
#define R2	PB2
#define R3	PB3
#define R4	PB4
#define R5	PB5
#define R6	PD6
#define R7	PD7

/* Includes ---------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>         // C library. Needed for conversion function
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "lcd.h"
#include "sinus_gen.h"

int main(void)
{
	/*Timer/Counter 0 settings --------------------------------------------------------------*/
	TIM0_set_mode_CTC();			// Set timer to CTC mode
	TIM0_overflow_16us();
	TIM0_CTC_A_interrupt_enable();	// Interrupt enable
	OCR0A = COMP_REG_A_MASK;		// Set Compare register A mask for 1 MHz frequency
	GPIO_config_output(&DDRC, PC0);
	GPIO_write_low(&PORTC, PC0);
	
	// SET output pins for R2R ladder
	GPIO_config_output(&DDRB, R0);
	GPIO_write_low(&PORTB, R0);
	GPIO_config_output(&DDRB, R1);
	GPIO_write_low(&PORTB, R1);
	GPIO_config_output(&DDRB, R2);
	GPIO_write_low(&PORTB, R2);
	GPIO_config_output(&DDRB, R3);
	GPIO_write_low(&PORTB, R3);
	GPIO_config_output(&DDRB, R4);
	GPIO_write_low(&PORTB, R4);
	GPIO_config_output(&DDRB, R5);
	GPIO_write_low(&PORTB, R5);
	GPIO_config_output(&DDRD, R6);
	GPIO_write_low(&PORTD, R6);
	GPIO_config_output(&DDRD, R7);
	GPIO_write_low(&PORTD, R7);
	
    sei();
    while (1) 
    {
    }
}
ISR(TIMER0_COMPA_vect)
{
	static uint8_t signal_amplitude = 0b00000000;		// variable for final amplitude
	//static uint16_t sample_cnt_1 = 0b00000000;
	static uint8_t lookup_table[128] =
	{
		0b01111111,    // sample1
		0b10000001,    // sample2
		0b10000010,    // sample3
		0b10000100,    // sample4
		0b10000101,    // sample5
		0b10000111,    // sample6
		0b10001000,    // sample7
		0b10001010,    // sample8
		0b10001011,    // sample9
		0b10001101,    // sample10
		0b10001111,    // sample11
		0b10010000,    // sample12
		0b10010010,    // sample13
		0b10010011,    // sample14
		0b10010101,    // sample15
		0b10010110,    // sample16
		0b10011000,    // sample17
		0b10011001,    // sample18
		0b10011011,    // sample19
		0b10011100,    // sample20
		0b10011110,    // sample21
		0b10011111,    // sample22
		0b10100001,    // sample23
		0b10100010,    // sample24
		0b10100100,    // sample25
		0b10100101,    // sample26
		0b10100111,    // sample27
		0b10101000,    // sample28
		0b10101010,    // sample29
		0b10101011,    // sample30
		0b10101101,    // sample31
		0b10101110,    // sample32
		0b10110000,    // sample33
		0b10110001,    // sample34
		0b10110010,    // sample35
		0b10110100,    // sample36
		0b10110101,    // sample37
		0b10110111,    // sample38
		0b10111000,    // sample39
		0b10111001,    // sample40
		0b10111011,    // sample41
		0b10111100,    // sample42
		0b10111110,    // sample43
		0b10111111,    // sample44
		0b11000000,    // sample45
		0b11000010,    // sample46
		0b11000011,    // sample47
		0b11000100,    // sample48
		0b11000110,    // sample49
		0b11000111,    // sample50
		0b11001000,    // sample51
		0b11001001,    // sample52
		0b11001011,    // sample53
		0b11001100,    // sample54
		0b11001101,    // sample55
		0b11001110,    // sample56
		0b11010000,    // sample57
		0b11010001,    // sample58
		0b11010010,    // sample59
		0b11010011,    // sample60
		0b11010100,    // sample61
		0b11010101,    // sample62
		0b11010111,    // sample63
		0b11011000,    // sample64
		0b11011001,    // sample65
		0b11011010,    // sample66
		0b11011011,    // sample67
		0b11011100,    // sample68
		0b11011101,    // sample69
		0b11011110,    // sample70
		0b11011111,    // sample71
		0b11100000,    // sample72
		0b11100001,    // sample73
		0b11100010,    // sample74
		0b11100011,    // sample75
		0b11100100,    // sample76
		0b11100101,    // sample77
		0b11100110,    // sample78
		0b11100111,    // sample79
		0b11101000,    // sample80
		0b11101001,    // sample81
		0b11101001,    // sample82
		0b11101010,    // sample83
		0b11101011,    // sample84
		0b11101100,    // sample85
		0b11101101,    // sample86
		0b11101110,    // sample87
		0b11101110,    // sample88
		0b11101111,    // sample89
		0b11110000,    // sample90
		0b11110000,    // sample91
		0b11110001,    // sample92
		0b11110010,    // sample93
		0b11110010,    // sample94
		0b11110011,    // sample95
		0b11110100,    // sample96
		0b11110100,    // sample97
		0b11110101,    // sample98
		0b11110101,    // sample99
		0b11110110,    // sample100
		0b11110111,    // sample101
		0b11110111,    // sample102
		0b11111000,    // sample103
		0b11111000,    // sample104
		0b11111001,    // sample105
		0b11111001,    // sample106
		0b11111001,    // sample107
		0b11111010,    // sample108
		0b11111010,    // sample109
		0b11111011,    // sample110
		0b11111011,    // sample111
		0b11111011,    // sample112
		0b11111100,    // sample113
		0b11111100,    // sample114
		0b11111100,    // sample115
		0b11111100,    // sample116
		0b11111101,    // sample117
		0b11111101,    // sample118
		0b11111101,    // sample119
		0b11111101,    // sample120
		0b11111101,    // sample121
		0b11111110,    // sample122
		0b11111110,    // sample123
		0b11111110,    // sample124
		0b11111110,    // sample125
		0b11111110,    // sample126
		0b11111110,    // sample127
		0b11111110,    // sample128
	};
	
	uint16_t sample_out = 0;
	static uint16_t sample_cnt_1 = 0;
	
	static uint16_t generovanafrekvence = 1000;
	
	uint16_t sample = 100000/generovanafrekvence;	
	sample_out = (512*sample_cnt_1/sample);  // vzorek ktery se ma aktualne vycist
	
	
	sample_cnt_1 ++;
	if(sample_cnt_1 >= sample){
		sample_cnt_1 = 0;		
	}
	
	//signal_amplitude = sample_out/2;
	signal_amplitude = sinus_gen(&lookup_table, sample_out);
	
	/*
	sample_cnt_1 ++;
	if(sample_cnt_1 > 512)
		sample_cnt_1 = 0;
	*/
		
	//GPIO_toggle(&PORTC, PC0);
	
	PORTB = signal_amplitude & 0b00111111;
	PORTD = signal_amplitude & 0b11000000;
	
}

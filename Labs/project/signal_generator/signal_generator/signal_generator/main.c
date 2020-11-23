/*
 * signal_generator.c
 *
 * Created: 19.11.2020 19:20:24
 * Author : Hula Josef (212557), Elias Martin (...)
 */ 
/*  Defines -------------------------------------------------------------------------- */
#define OCR0A_TOP_MASK		0b00010000

#define
#ifndef F_CPU
#define F_CPU			16000000	// CPU frequency in Hz 
#endif


#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "timer.h"

// Set TIMER 0 to overflow when counter reaches TOP value and enable interrupt
// Select mode of operation CTC in WGM02:0 bits in TCCR0B register
TIM0_CTC_mode_set(); 

// Set output Compare Register OCR0A to COMP_TOP value
OCR0A = OCR0A_TOP_MASK;

// Enable corresponding interrupt (Output compare interrupt)
TIM0_compare_match_A_interrupt_enable();

// Global interrupt enabled
sei();

uint8_t sinus[128] = 
{
0b	00000000	;	// sample  0
0b	00000010	;	// sample  1
0b	00000011	;	// sample  2
0b	00000101	;	// sample  3
0b	00000110	;	// sample  4
0b	00001000	;	// sample  5
0b	00001001	;	// sample  6
0b	00001011	;	// sample  7
0b	00001101	;	// sample  8
0b	00001110	;	// sample  9
0b	00010000	;	// sample  10
0b	00010001	;	// sample  11
0b	00010011	;	// sample  12
0b	00010100	;	// sample  13
0b	00010110	;	// sample  14
0b	00010111	;	// sample  15
0b	00011001	;	// sample  16
0b	00011011	;	// sample  17
0b	00011100	;	// sample  18
0b	00011110	;	// sample  19
0b	00011111	;	// sample  20
0b	00100001	;	// sample  21
0b	00100010	;	// sample  22
0b	00100100	;	// sample  23
0b	00100101	;	// sample  24
0b	00100111	;	// sample  25
0b	00101000	;	// sample  26
0b	00101010	;	// sample  27
0b	00101011	;	// sample  28
0b	00101101	;	// sample  29
0b	00101110	;	// sample  30
0b	00110000	;	// sample  31
0b	00110001	;	// sample  32
0b	00110010	;	// sample  33
0b	00110100	;	// sample  34
0b	00110101	;	// sample  35
0b	00110111	;	// sample  36
0b	00111000	;	// sample  37
0b	00111010	;	// sample  38
0b	00111011	;	// sample  39
0b	00111100	;	// sample  40
0b	00111110	;	// sample  41
0b	00111111	;	// sample  42
0b	01000000	;	// sample  43
0b	01000010	;	// sample  44
0b	01000011	;	// sample  45
0b	01000100	;	// sample  46
0b	01000110	;	// sample  47
0b	01000111	;	// sample  48
0b	01001000	;	// sample  49
0b	01001010	;	// sample  50
0b	01001011	;	// sample  51
0b	01001100	;	// sample  52
0b	01001110	;	// sample  53
0b	01001111	;	// sample  54
0b	01010000	;	// sample  55
0b	01010001	;	// sample  56
0b	01010010	;	// sample  57
0b	01010100	;	// sample  58
0b	01010101	;	// sample  59
0b	01010110	;	// sample  60
0b	01010111	;	// sample  61
0b	01011000	;	// sample  62
0b	01011001	;	// sample  63
0b	01011011	;	// sample  64
0b	01011100	;	// sample  65
0b	01011101	;	// sample  66
0b	01011110	;	// sample  67
0b	01011111	;	// sample  68
0b	01100000	;	// sample  69
0b	01100001	;	// sample  70
0b	01100010	;	// sample  71
0b	01100011	;	// sample  72
0b	01100100	;	// sample  73
0b	01100101	;	// sample  74
0b	01100110	;	// sample  75
0b	01100111	;	// sample  76
0b	01101000	;	// sample  77
0b	01101001	;	// sample  78
0b	01101010	;	// sample  79
0b	01101010	;	// sample  80
0b	01101011	;	// sample  81
0b	01101100	;	// sample  82
0b	01101101	;	// sample  83
0b	01101110	;	// sample  84
0b	01101111	;	// sample  85
0b	01101111	;	// sample  86
0b	01110000	;	// sample  87
0b	01110001	;	// sample  88
0b	01110010	;	// sample  89
0b	01110010	;	// sample  90
0b	01110011	;	// sample  91
0b	01110100	;	// sample  92
0b	01110100	;	// sample  93
0b	01110101	;	// sample  94
0b	01110110	;	// sample  95
0b	01110110	;	// sample  96
0b	01110111	;	// sample  97
0b	01110111	;	// sample  98
0b	01111000	;	// sample  99
0b	01111001	;	// sample  100
0b	01111001	;	// sample  101
0b	01111010	;	// sample  102
0b	01111010	;	// sample  103
0b	01111010	;	// sample  104
0b	01111011	;	// sample  105
0b	01111011	;	// sample  106
0b	01111100	;	// sample  107
0b	01111100	;	// sample  108
0b	01111101	;	// sample  109
0b	01111101	;	// sample  110
0b	01111101	;	// sample  111
0b	01111110	;	// sample  112
0b	01111110	;	// sample  113
0b	01111110	;	// sample  114
0b	01111110	;	// sample  115
0b	01111111	;	// sample  116
0b	01111111	;	// sample  117
0b	01111111	;	// sample  118
0b	01111111	;	// sample  119
0b	01111111	;	// sample  120
0b	10000000	;	// sample  121
0b	10000000	;	// sample  122
0b	10000000	;	// sample  123
0b	10000000	;	// sample  124
0b	10000000	;	// sample  125
0b	10000000	;	// sample  126
0b	10000000	// sample  127
};
int main(void)
{
	
    while (1) 
    {
		
    }
}
ISR(TIMER0_COMPA_vect)
{
	
}

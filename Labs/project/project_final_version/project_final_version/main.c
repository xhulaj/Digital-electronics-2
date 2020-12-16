/*
 * project_final_version.c
 *
 * Created: 16.12.2020 0:57:27
 * Author : spravce
 */ 

/* Defines ----------------------------------------------------------*/
#ifndef F_CPU
#define F_CPU			16000000	// CPU frequency in Hz
#endif
#define COMP_REG_A_MASK 0b10100000;

#define R0	PD3
#define R1	PD0
#define R2	PB2
#define R3	PB3
#define R4	PB4
#define R5	PB5
#define R6	PD1
#define R7	PD2

#define F_INT 100000 //frequency of generator interrupt 99378
#define SIN_SAM 512 //number of saved samples of sinus (saved is only first half)
#define SIN_SAM4 128 //number of saved samples of sinus (saved is only first half)

/* Includes ---------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>         // C library. Needed for conversion function
#include "gpio.h"          // Gpio library
#include "timer.h"          // Timer library for AVR-GCC
#include "uart.h"           // Peter Fleury's UART library
#include "lcd.h"            // Peter Fleury's LCD library
#include "sinus_gen.h"
#include "debounce.h"

	uint8_t button = 0;			// temporary variable for setting frequency
	uint16_t gen_f1 = 1000; //frequency of first signal
	uint16_t gen_f2 = 1336; //frequency of second signal

	
int main(void)
{
		/* ADC settings ------------------------------------------------------
    Configure ADC to convert PC0[A0] analog value
    Set ADC reference to AVcc
	*/
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
	/* ADC settings ----------------------------------------------------------*/
	
    //timer for checking state of buttons every 4ms 
	TIM1_overflow_4ms(); //1s for debugging
	TIM1_overflow_interrupt_enable();
	
	// Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600,F_CPU));


	/*Timer/Counter 0 settings --------------------------------------------------------------*/
	TIM0_set_mode_CTC();			// Set timer to CTC mode
	TIM0_overflow_16us();
	TIM0_CTC_A_interrupt_enable();	// Interrupt enable
	OCR0A = COMP_REG_A_MASK;		// Set Compare register A mask for 1 MHz frequency
	
	// SET output pins for R2R ladder
	GPIO_config_output(&DDRD, R0);
	GPIO_write_low(&PORTD, R0);
	GPIO_config_output(&DDRD, R1);
	GPIO_write_low(&PORTD, R1);
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
	
	/* Managing process ---------------------------------------------*/
	// Initialize LCD display
	lcd_init(LCD_DISP_ON);
	lcd_gotoxy(0, 0); lcd_puts("FREQ1:");
	lcd_gotoxy(0, 1); lcd_puts("FREQ2:");
    // Enables interrupts by setting the global interrupt mask
    sei();
	while (1)
	{
		
	}
}
ISR(TIMER1_OVF_vect)
{
	char lcd_str_1 = "    ";
	char lcd_str_2 = "    ";
	itoa(gen_f1, lcd_str_1, 10);
	itoa(gen_f2, lcd_str_2, 10);
	lcd_gotoxy(10,0);
	lcd_puts(lcd_str_1);
	lcd_gotoxy(10,1);
	lcd_puts(lcd_str_2);
	// Start ADC conversion
	ADCSRA |= (1 << ADSC);
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
			
    /* Button recognition ------------------------------------------------------------*/
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
			gen_f1 = 810;
			gen_f2 = 1209;
		}		
	}
	else if (660 >= value && value > 610) //4
	{
		button_history[1] &= ~(1<<0); 
		if (is_button_pressed(&button_history[1]))
		{
			uart_puts("4");	
			gen_f1 = 880;
			gen_f2 = 1209;
		}		
	}
	else if (610 >= value && value > 570) //7
	{
		button_history[2] &= ~(1<<0); 
		if (is_button_pressed(&button_history[2]))
		{
			uart_puts("7");
			gen_f1 = 950;
			gen_f2 = 1209;
		}
	}
	else if (570 >= value && value > 530) //*
	{
		button_history[3] &= ~(1<<0); 
		if (is_button_pressed(&button_history[3]))
		{
			uart_puts("*");
			gen_f1 = 1020;
			gen_f2 = 1209;
		}
	}
	else if (530 >= value && value > 490) //2
	{
		button_history[4] &= ~(1<<0); 
		if (is_button_pressed(&button_history[4]))
		{
			uart_puts("2");
			gen_f1 = 810;
			gen_f2 = 1336;
		}
	}
	else if (490 >= value && value > 460) //3
	{
		button_history[5] &= 0xFFFE; 
		if (is_button_pressed(&button_history[5]))
		{
			uart_puts("3");
			gen_f1 = 810;
			gen_f2 = 1477;
		}
	}
	else if (460 >= value && value > 430) //A
	{
		button_history[6] &= ~(1<<0); 
		if (is_button_pressed(&button_history[6]))
		{
			uart_puts("A");
			gen_f1 = 810;
			gen_f2 = 1633;
		}
	}
    else if (430 >= value && value > 390) //5
    {
		button_history[7] &= ~(1<<0); 
		if (is_button_pressed(&button_history[7]))
		{
			uart_puts("5");
			gen_f1 = 880;
			gen_f2 = 1336;
		}
    }
	else if (390 >= value && value > 340) //6
	{
		button_history[8] &= ~(1<<0); 
		if (is_button_pressed(&button_history[8]))
		{
			uart_puts("6");
			gen_f1 = 880;
 			gen_f2 = 1477;
		}
	}
	else if (340 >= value && value > 290) //B
	{
		button_history[9] &= ~(1<<0); 
		if (is_button_pressed(&button_history[9]))
		{
			uart_puts("B");
			gen_f1 = 880;
			gen_f2 = 1633;
		}
	}
	else if (290 >= value && value > 230) //8
	{
		button_history[10] &= ~(1<<0); 
		if (is_button_pressed(&button_history[10]))
		{
			uart_puts("8");
			gen_f1 = 950;
			gen_f2 = 1336;
		}
	}
	else if (230 >= value && value > 190) //9
	{
		button_history[11] &= ~(1<<0); 
		if (is_button_pressed(&button_history[11]))
		{
			uart_puts("9");
			gen_f1 = 950;
			gen_f2 = 1477;
		}
	}
	else if (190 >= value && value > 150) //0
	{
		button_history[12] &= ~(1<<0); 
		if (is_button_pressed(&button_history[12]))
		{
			uart_puts("0");
			gen_f1 = 1020;
			gen_f2 = 1336;
		}
	}
	else if (150 >= value && value > 110) //C
	{
		button_history[13] &= ~(1<<0); 
		if (is_button_pressed(&button_history[13]))
		{
			uart_puts("C");
			gen_f1 = 950;
			gen_f2 = 1633;
		}
	}
	else if (110 >= value && value > 50) //#
	{
		button_history[14] &= ~(1<<0); 
		if (is_button_pressed(&button_history[14]))
		{
			uart_puts("#");
			gen_f1 = 1020;
			gen_f2 = 1477;
		}
	}
	else if (50 >= value && value >= 0) //D
	{
		button_history[15] &= ~(1<<0); 
		if (is_button_pressed(&button_history[15]))
		{
			uart_puts("D");
			gen_f1 = 1020;
			gen_f2 = 1633;
		}
	}
}
ISR(TIMER0_COMPA_vect)
{
	
	static uint8_t signal_amplitude = 0b00000000; //final amplitude
	//uint8_t num_of_per_sam_sin = SIN_SAM/ 4; //number of samples of full period of sinus
	
	//quarter of sine saved in table
	static uint8_t lookup_table[SIN_SAM4] =
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
	
	
	uint16_t sample = F_INT/gen_f1; //num of samples that will by in one period of generated signal
	uint16_t sample2 = F_INT/gen_f2; //num of samples that will by in one period of generated signal
	
	uint16_t sample_out = 0; // sample that will be read from table
	uint16_t sample_out2 = 0; // sample that will be read from table
	
	static uint16_t sample_cnt_1 = 0; //counter
	static uint16_t sample_cnt_2 = 0; //counter
	
	
	
	sample_out = (SIN_SAM*sample_cnt_1/sample); // sample that will be read from table
	sample_out2 = (SIN_SAM*sample_cnt_2/sample2); // sample that will be read from table
	
	//reseting counter
	sample_cnt_1 ++;
	if(sample_cnt_1 >= sample){
		sample_cnt_1 = 0;
	}
	//reseting counter
	sample_cnt_2 ++;
	if(sample_cnt_2 >= sample2){
		sample_cnt_2 = 0;
	}
	
	//signal_amplitude = sample_out/2; //generovani pila
	signal_amplitude = (sinus_gen(&lookup_table, sample_out) + sinus_gen(&lookup_table, sample_out2))/2; //generovani sinus
	//if(sample_out2 ==  )
	//signal_amplitude =  //generovani obdelnik
	
	GPIO_set_pin(&PORTD, R0, bit_val(signal_amplitude, 1));
	GPIO_set_pin(&PORTD, R1, bit_val(signal_amplitude, 2));
	GPIO_set_pin(&PORTB, R2, bit_val(signal_amplitude, 3));
	GPIO_set_pin(&PORTB, R3, bit_val(signal_amplitude, 4));
	GPIO_set_pin(&PORTB, R4, bit_val(signal_amplitude, 5));
	GPIO_set_pin(&PORTB, R5, bit_val(signal_amplitude, 6));
	GPIO_set_pin(&PORTD, R6, bit_val(signal_amplitude, 7));
	GPIO_set_pin(&PORTD, R7, bit_val(signal_amplitude, 8));
	
}
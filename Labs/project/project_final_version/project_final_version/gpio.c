/***********************************************************************
 * 
 * GPIO library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include "gpio.h"

/* Function definitions ----------------------------------------------*/

/* GPIO_config_output */
void GPIO_config_output(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name | (1<<pin_num); // Configurate output in register on given pin
}

/*--------------------------------------------------------------------*/

/* GPIO_config_input_nopull */
void GPIO_config_input_nopull(volatile uint8_t *reg_name, uint8_t pin_num)
{
	*reg_name = *reg_name & ~(1<<pin_num); //DDR
	*reg_name++;						   // change register to PORT
	*reg_name = *reg_name & ~(1<<pin_num);
}

/*--------------------------------------------------------------------*/

/* GPIO_config_input_pullup */
void GPIO_config_input_pullup(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name & ~(1<<pin_num);  // Data Direction Register
    *reg_name++;                    // Change pointer to Data Register
    *reg_name = *reg_name | (1<<pin_num);   // Data Register
}

/*--------------------------------------------------------------------*/

/* GPIO_write_low */
void GPIO_write_low(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name & ~(1<<pin_num); // Write low-value to a set bit in given register
}

/*--------------------------------------------------------------------*/

/* GPIO_write_high */
void GPIO_write_high(volatile uint8_t *reg_name, uint8_t pin_num)
{
	*reg_name |= (1<<pin_num); //Write high-value to a set bit in given register
}
/*--------------------------------------------------------------------*/

/* GPIO_toggle */
void GPIO_toggle(volatile uint8_t *reg_name, uint8_t pin_num)
{
	*reg_name ^= (1<<pin_num);  // Flib given bit in register
}


/*--------------------------------------------------------------------*/

/* GPIO_read */
uint8_t GPIO_read(volatile uint8_t *reg_name, uint8_t pin_num)
{
	return(bit_is_set(*reg_name, pin_num)); // if bit is set, returns 1, if not, returns 0
	
}

uint8_t GPIO_set_pin(volatile uint8_t *reg_name, uint8_t pin_num, uint8_t state)
{
	if(state & (1 << 0))
	*reg_name |= (1<<pin_num); //Write high-value to a set bit in given register
	else
	*reg_name = *reg_name & ~(1<<pin_num); // Write low-value to a set bit in given register
}

uint8_t bit_val(uint8_t var, uint8_t pos)
{
	if (var & (1 << (pos - 1)))
	return 1;
	else
	return 0;
}
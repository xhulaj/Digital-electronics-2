/*
 * debounce.c
 *
 * Created: 21.11.2020 21:36:38
 *  Author: Elias
 */ 

//for pull-up    number of zeros * ISR period is debounce time
#define MASK     0b1111000000111111


/* Includes ----------------------------------------------------------*/
#include "debounce.h"

/* Function definitions ----------------------------------------------*/

uint16_t is_button_pressed(uint16_t *button_history){
	uint8_t pressed = 0;
	if ((*button_history & MASK) == 0b1111000000000000){ 
		pressed = 1;
		*button_history = 0b0000000000000000;
	}
	return pressed;
}


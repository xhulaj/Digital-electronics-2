/*
 * debounce.h
 *
 * Created: 21.11.2020 21:37:06
 *  Author: Elias
 */ 


#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#include <avr/io.h>

uint16_t is_button_pressed(uint16_t *button_history);




#endif /* DEBOUNCE_H_ */
/*
 * sinus_gen.h
 *
 * Created: 15.12.2020 16:46:00
 *  Author: spravce
 */ 
#include <stdlib.h>         // C library. Needed for conversion function
#include <avr/io.h>
#ifndef SINUS_GEN_H_
#define SINUS_GEN_H_

uint8_t sinus_gen(uint8_t *table, uint16_t sample);

#endif /* SINUS_GEN_H_ */
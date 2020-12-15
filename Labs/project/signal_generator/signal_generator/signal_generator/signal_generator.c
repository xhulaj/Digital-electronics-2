/*
 * signal_generator.c
 *
 * Created: 25.11.2020 17:54:21
 *  Author: Josef Hula
 */ 

/* Includes----------------------------------------------------*/
#include "signal_generator.h"

/* Function definitions ----------------------------------------*/

uint16_t gen_sig_sample_id(uint16_t *cnt, uint16_t freq)
{
	uint8_t sample = 512*(195/freq);
	uint16_t sample_out = ((*cnt/sample)*512) ;  // vypocet vzorku, ktery se ma zpracovat. Jelikoz je pouzity jako index v lookup table
	if(*cnt >= sample) *cnt = 0;
	return sample_out;
}

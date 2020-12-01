/*
 * signal_generator.c
 *
 * Created: 25.11.2020 17:54:21
 *  Author: Josef Hula
 */ 

/* Includes----------------------------------------------------*/
#include "signal_generator.h"
#define F_TIM0			1000000;

/* Function definitions ----------------------------------------*/

uint16_t gen_sig_sample_id(uint32_t *sample, uint16_t *sample_vol)
{
	uint16_t table_sample = ((512 * (*sample))/ *sample_vol) ;  // vypocet vzorku, ktery se ma zpracovat. Jelikoz je pouzity jako index v lookup table
	if(*sample >= sample_vol) *sample = 0;
	return table_sample;
}
uint16_t gen_sample_quantity(uint16_t frequency)
{
	return (F_TIM0 / frequency);
}
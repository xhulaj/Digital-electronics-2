/*
 * signal_generator.c
 *
 * Created: 25.11.2020 17:54:21
 *  Author: Josef Hula
 */ 

/* Includes----------------------------------------------------*/
#include "signal_generator.h"

/* Function definitions ----------------------------------------*/

uint16_t gen_sig_sample_id(uint32_t sample, uint16_t *frequency)
{
	uint16_t table_sample = ((512 * sample)/ *frequency) % 512;
	return table_sample;
}

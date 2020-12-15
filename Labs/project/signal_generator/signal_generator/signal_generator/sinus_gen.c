/*
 * sinus_gen.c
 *
 * Created: 15.12.2020 16:44:56
 *  Author: spravce
 */ 
#include "sinus_gen.h"

uint8_t sinus_gen(uint8_t *table, uint16_t sample)
{
	uint8_t out = 0;
	if((sample >= 127) && (sample <= 255))
	{
		out = table[255-sample];
	}
	else if ((sample >= 256) && (sample <= 383))
	{
		out = -1*(table[sample - 256]);
	}
	else if((sample >= 384) && (sample <= 511))
	{
		out = -1 * (table[511 - sample]);
	}
	else
	{
		out = table[sample];
	}
	return out;
}
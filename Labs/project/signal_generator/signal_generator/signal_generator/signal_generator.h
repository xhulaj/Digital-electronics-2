/*
 * signal_generator.h
 *
 * Created: 25.11.2020 17:35:40
 *  Author: Josef Hula
 */ 
#include <avr/io.h>

#ifndef SIGNAL_GENERATOR_H_
#define SIGNAL_GENERATOR_H_

uint16_t gen_sig_sample_id(uint32_t sample, uint16_t *frequency);

#endif /* SIGNAL_GENERATOR_H_ */
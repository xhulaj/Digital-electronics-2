/*
 * signal_generator.c
 *
 * Created: 19.11.2020 19:20:24
 * Author : Hula Josef (212557), Elias Martin (...)
 */ 
/*  Defines -------------------------------------------------------------------------- */


#ifndef F_CPU
#define F_CPU			16000000	// CPU frequency in Hz 
#endif
#define R0	PB0
#define R1	PB1
#define R2	PB2
#define R3	PB3
#define R4	PB4
#define R5	PB5
#define R6	PD6
#define R7	PD7

// Definition of output compare register value for 1MHz frequency of 
#define COMP_REG_A_MASK 0b00011111;
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>         // C library. Needed for conversion function
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "lcd.h"
#include "signal_generator.h"



int main(void)
{
	//############# TIMER/COUNTER0 SETTINGS
	TIM0_set_mode_CTC();			// Set timer to CTC mode
	TIM0_overflow_16us();
	TIM0_CTC_A_interrupt_enable();	// Interrupt enable
	OCR0A = COMP_REG_A_MASK;		// Set Compare register A mask for 1 MHz frequency
	GPIO_config_output(&DDRC, PC0);
	GPIO_write_low(&PORTC, PC0);
	// SET output pins for R2R ladder
	GPIO_config_output(&DDRB, R0);
	GPIO_write_low(&PORTB, R0);
	GPIO_config_output(&DDRB, R1);
	GPIO_write_low(&PORTB, R1);
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
	
	// Enables interrupts by setting the global interrupt mask
	sei();
    while (1) 
    {

    }
}

ISR(TIMER0_COMPA_vect)
{
	//char uart_message[4] = "    ";
	static uint8_t key = 00;
	static uint8_t signal_amplitude = 0b00000000;		// variable for final amplitude
	static uint16_t sample_cnt_1 = 0b00000000;
	static uint16_t sample_cnt_2 = 0b00000000;
// Table of set frequencies, whose values are for interrupt frequency 1 MHz equal to their number of samples
	static uint16_t samples_set[8] =
	{
		1435,//697
		1298,//770
		1212,//825,
		1063,//941
		827,//1209
		1336,
		1477,
		1633
	};
	
// Table for quarter of sinus wave
	static uint8_t sinus_table[512] =
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
		0b11111110,    // sample129
		0b11111110,    // sample130
		0b11111110,    // sample131
		0b11111110,    // sample132
		0b11111110,    // sample133
		0b11111110,    // sample134
		0b11111110,    // sample135
		0b11111110,    // sample136
		0b11111101,    // sample137
		0b11111101,    // sample138
		0b11111101,    // sample139
		0b11111101,    // sample140
		0b11111101,    // sample141
		0b11111100,    // sample142
		0b11111100,    // sample143
		0b11111100,    // sample144
		0b11111100,    // sample145
		0b11111011,    // sample146
		0b11111011,    // sample147
		0b11111011,    // sample148
		0b11111010,    // sample149
		0b11111010,    // sample150
		0b11111001,    // sample151
		0b11111001,    // sample152
		0b11111001,    // sample153
		0b11111000,    // sample154
		0b11111000,    // sample155
		0b11110111,    // sample156
		0b11110111,    // sample157
		0b11110110,    // sample158
		0b11110101,    // sample159
		0b11110101,    // sample160
		0b11110100,    // sample161
		0b11110100,    // sample162
		0b11110011,    // sample163
		0b11110010,    // sample164
		0b11110010,    // sample165
		0b11110001,    // sample166
		0b11110000,    // sample167
		0b11110000,    // sample168
		0b11101111,    // sample169
		0b11101110,    // sample170
		0b11101110,    // sample171
		0b11101101,    // sample172
		0b11101100,    // sample173
		0b11101011,    // sample174
		0b11101010,    // sample175
		0b11101001,    // sample176
		0b11101001,    // sample177
		0b11101000,    // sample178
		0b11100111,    // sample179
		0b11100110,    // sample180
		0b11100101,    // sample181
		0b11100100,    // sample182
		0b11100011,    // sample183
		0b11100010,    // sample184
		0b11100001,    // sample185
		0b11100000,    // sample186
		0b11011111,    // sample187
		0b11011110,    // sample188
		0b11011101,    // sample189
		0b11011100,    // sample190
		0b11011011,    // sample191
		0b11011010,    // sample192
		0b11011001,    // sample193
		0b11011000,    // sample194
		0b11010111,    // sample195
		0b11010101,    // sample196
		0b11010100,    // sample197
		0b11010011,    // sample198
		0b11010010,    // sample199
		0b11010001,    // sample200
		0b11010000,    // sample201
		0b11001110,    // sample202
		0b11001101,    // sample203
		0b11001100,    // sample204
		0b11001011,    // sample205
		0b11001001,    // sample206
		0b11001000,    // sample207
		0b11000111,    // sample208
		0b11000110,    // sample209
		0b11000100,    // sample210
		0b11000011,    // sample211
		0b11000010,    // sample212
		0b11000000,    // sample213
		0b10111111,    // sample214
		0b10111110,    // sample215
		0b10111100,    // sample216
		0b10111011,    // sample217
		0b10111001,    // sample218
		0b10111000,    // sample219
		0b10110111,    // sample220
		0b10110101,    // sample221
		0b10110100,    // sample222
		0b10110010,    // sample223
		0b10110001,    // sample224
		0b10110000,    // sample225
		0b10101110,    // sample226
		0b10101101,    // sample227
		0b10101011,    // sample228
		0b10101010,    // sample229
		0b10101000,    // sample230
		0b10100111,    // sample231
		0b10100101,    // sample232
		0b10100100,    // sample233
		0b10100010,    // sample234
		0b10100001,    // sample235
		0b10011111,    // sample236
		0b10011110,    // sample237
		0b10011100,    // sample238
		0b10011011,    // sample239
		0b10011001,    // sample240
		0b10011000,    // sample241
		0b10010110,    // sample242
		0b10010101,    // sample243
		0b10010011,    // sample244
		0b10010010,    // sample245
		0b10010000,    // sample246
		0b10001111,    // sample247
		0b10001101,    // sample248
		0b10001011,    // sample249
		0b10001010,    // sample250
		0b10001000,    // sample251
		0b10000111,    // sample252
		0b10000101,    // sample253
		0b10000100,    // sample254
		0b10000010,    // sample255
		0b10000001,    // sample256
		0b01111111,    // sample257
		0b01111101,    // sample258
		0b01111100,    // sample259
		0b01111010,    // sample260
		0b01111001,    // sample261
		0b01110111,    // sample262
		0b01110110,    // sample263
		0b01110100,    // sample264
		0b01110011,    // sample265
		0b01110001,    // sample266
		0b01101111,    // sample267
		0b01101110,    // sample268
		0b01101100,    // sample269
		0b01101011,    // sample270
		0b01101001,    // sample271
		0b01101000,    // sample272
		0b01100110,    // sample273
		0b01100101,    // sample274
		0b01100011,    // sample275
		0b01100010,    // sample276
		0b01100000,    // sample277
		0b01011111,    // sample278
		0b01011101,    // sample279
		0b01011100,    // sample280
		0b01011010,    // sample281
		0b01011001,    // sample282
		0b01010111,    // sample283
		0b01010110,    // sample284
		0b01010100,    // sample285
		0b01010011,    // sample286
		0b01010001,    // sample287
		0b01010000,    // sample288
		0b01001110,    // sample289
		0b01001101,    // sample290
		0b01001100,    // sample291
		0b01001010,    // sample292
		0b01001001,    // sample293
		0b01000111,    // sample294
		0b01000110,    // sample295
		0b01000101,    // sample296
		0b01000011,    // sample297
		0b01000010,    // sample298
		0b01000000,    // sample299
		0b00111111,    // sample300
		0b00111110,    // sample301
		0b00111100,    // sample302
		0b00111011,    // sample303
		0b00111010,    // sample304
		0b00111000,    // sample305
		0b00110111,    // sample306
		0b00110110,    // sample307
		0b00110101,    // sample308
		0b00110011,    // sample309
		0b00110010,    // sample310
		0b00110001,    // sample311
		0b00110000,    // sample312
		0b00101110,    // sample313
		0b00101101,    // sample314
		0b00101100,    // sample315
		0b00101011,    // sample316
		0b00101010,    // sample317
		0b00101001,    // sample318
		0b00100111,    // sample319
		0b00100110,    // sample320
		0b00100101,    // sample321
		0b00100100,    // sample322
		0b00100011,    // sample323
		0b00100010,    // sample324
		0b00100001,    // sample325
		0b00100000,    // sample326
		0b00011111,    // sample327
		0b00011110,    // sample328
		0b00011101,    // sample329
		0b00011100,    // sample330
		0b00011011,    // sample331
		0b00011010,    // sample332
		0b00011001,    // sample333
		0b00011000,    // sample334
		0b00010111,    // sample335
		0b00010110,    // sample336
		0b00010101,    // sample337
		0b00010101,    // sample338
		0b00010100,    // sample339
		0b00010011,    // sample340
		0b00010010,    // sample341
		0b00010001,    // sample342
		0b00010000,    // sample343
		0b00010000,    // sample344
		0b00001111,    // sample345
		0b00001110,    // sample346
		0b00001110,    // sample347
		0b00001101,    // sample348
		0b00001100,    // sample349
		0b00001100,    // sample350
		0b00001011,    // sample351
		0b00001010,    // sample352
		0b00001010,    // sample353
		0b00001001,    // sample354
		0b00001001,    // sample355
		0b00001000,    // sample356
		0b00000111,    // sample357
		0b00000111,    // sample358
		0b00000110,    // sample359
		0b00000110,    // sample360
		0b00000101,    // sample361
		0b00000101,    // sample362
		0b00000101,    // sample363
		0b00000100,    // sample364
		0b00000100,    // sample365
		0b00000011,    // sample366
		0b00000011,    // sample367
		0b00000011,    // sample368
		0b00000010,    // sample369
		0b00000010,    // sample370
		0b00000010,    // sample371
		0b00000010,    // sample372
		0b00000001,    // sample373
		0b00000001,    // sample374
		0b00000001,    // sample375
		0b00000001,    // sample376
		0b00000001,    // sample377
		0b00000000,    // sample378
		0b00000000,    // sample379
		0b00000000,    // sample380
		0b00000000,    // sample381
		0b00000000,    // sample382
		0b00000000,    // sample383
		0b00000000,    // sample384
		0b00000000,    // sample385
		0b00000000,    // sample386
		0b00000000,    // sample387
		0b00000000,    // sample388
		0b00000000,    // sample389
		0b00000000,    // sample390
		0b00000000,    // sample391
		0b00000000,    // sample392
		0b00000001,    // sample393
		0b00000001,    // sample394
		0b00000001,    // sample395
		0b00000001,    // sample396
		0b00000001,    // sample397
		0b00000010,    // sample398
		0b00000010,    // sample399
		0b00000010,    // sample400
		0b00000010,    // sample401
		0b00000011,    // sample402
		0b00000011,    // sample403
		0b00000011,    // sample404
		0b00000100,    // sample405
		0b00000100,    // sample406
		0b00000101,    // sample407
		0b00000101,    // sample408
		0b00000101,    // sample409
		0b00000110,    // sample410
		0b00000110,    // sample411
		0b00000111,    // sample412
		0b00000111,    // sample413
		0b00001000,    // sample414
		0b00001001,    // sample415
		0b00001001,    // sample416
		0b00001010,    // sample417
		0b00001010,    // sample418
		0b00001011,    // sample419
		0b00001100,    // sample420
		0b00001100,    // sample421
		0b00001101,    // sample422
		0b00001110,    // sample423
		0b00001110,    // sample424
		0b00001111,    // sample425
		0b00010000,    // sample426
		0b00010000,    // sample427
		0b00010001,    // sample428
		0b00010010,    // sample429
		0b00010011,    // sample430
		0b00010100,    // sample431
		0b00010101,    // sample432
		0b00010101,    // sample433
		0b00010110,    // sample434
		0b00010111,    // sample435
		0b00011000,    // sample436
		0b00011001,    // sample437
		0b00011010,    // sample438
		0b00011011,    // sample439
		0b00011100,    // sample440
		0b00011101,    // sample441
		0b00011110,    // sample442
		0b00011111,    // sample443
		0b00100000,    // sample444
		0b00100001,    // sample445
		0b00100010,    // sample446
		0b00100011,    // sample447
		0b00100100,    // sample448
		0b00100101,    // sample449
		0b00100110,    // sample450
		0b00100111,    // sample451
		0b00101001,    // sample452
		0b00101010,    // sample453
		0b00101011,    // sample454
		0b00101100,    // sample455
		0b00101101,    // sample456
		0b00101110,    // sample457
		0b00110000,    // sample458
		0b00110001,    // sample459
		0b00110010,    // sample460
		0b00110011,    // sample461
		0b00110101,    // sample462
		0b00110110,    // sample463
		0b00110111,    // sample464
		0b00111000,    // sample465
		0b00111010,    // sample466
		0b00111011,    // sample467
		0b00111100,    // sample468
		0b00111110,    // sample469
		0b00111111,    // sample470
		0b01000000,    // sample471
		0b01000010,    // sample472
		0b01000011,    // sample473
		0b01000101,    // sample474
		0b01000110,    // sample475
		0b01000111,    // sample476
		0b01001001,    // sample477
		0b01001010,    // sample478
		0b01001100,    // sample479
		0b01001101,    // sample480
		0b01001110,    // sample481
		0b01010000,    // sample482
		0b01010001,    // sample483
		0b01010011,    // sample484
		0b01010100,    // sample485
		0b01010110,    // sample486
		0b01010111,    // sample487
		0b01011001,    // sample488
		0b01011010,    // sample489
		0b01011100,    // sample490
		0b01011101,    // sample491
		0b01011111,    // sample492
		0b01100000,    // sample493
		0b01100010,    // sample494
		0b01100011,    // sample495
		0b01100101,    // sample496
		0b01100110,    // sample497
		0b01101000,    // sample498
		0b01101001,    // sample499
		0b01101011,    // sample500
		0b01101100,    // sample501
		0b01101110,    // sample502
		0b01101111,    // sample503
		0b01110001,    // sample504
		0b01110011,    // sample505
		0b01110100,    // sample506
		0b01110110,    // sample507
		0b01110111,    // sample508
		0b01111001,    // sample509
		0b01111010,    // sample510
		0b01111100,    // sample511
		0b01111101     // sample512
	};
		// return amplitude value based on currently pressed button
		
		//signal_amplitude = (sample_cnt_1);				  // sawwtooth signal generation
		signal_amplitude = sinus_table[sample_cnt_1];	  // sinus gegeration test
		//keys are index from 0 to 15, thus row frquency (index 0-3) is given by: key/4; and and column frequency (index 4-7)  is given by: 4 + key % 4;
		//signal_amplitude = (sinus_table[gen_sig_sample_id(&sample_cnt_1, &samples_set[key/4])] + sinus_table[gen_sig_sample_id(&sample_cnt_2, &samples_set[4 + (key % 4)])])/2;  
		sample_cnt_1 = sample_cnt_1 + 1;
		sample_cnt_2 = sample_cnt_2 + 1;
		if(sample_cnt_1 > 511) sample_cnt_1 = 0;		  // reseting counter, test purposes
		PORTB = signal_amplitude & 0b00111111;
		PORTD = signal_amplitude & 0b11000000;
}



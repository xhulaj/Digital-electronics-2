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

// Definition of output compare register value for 1MHz frequency of 
#define COMP_REG_A_MASK 0b00000111;

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
	//uint8_t output = 0b00000000;
	//############# TIMER/COUNTER0 SETTINGS
	TIM0_set_mode_CTC();			// Set timer to CTC mode
	TIM0_overflow_16us();
	TIM0_CTC_A_interrupt_enable();	// Interrupt enable
	OCR0A = COMP_REG_A_MASK;		// Set Compare register A mask for 1 MHz frequency
	GPIO_config_output(&DDRC, PC0);
	GPIO_write_low(&PORTC, PC0);
	
	// Enables interrupts by setting the global interrupt mask
	sei();
    while (1) 
    {
		
    }
}

ISR(TIMER0_COMPA_vect)
{
	char uart_message[4] = "    ";
	static uint8_t key = 12;
	static uint8_t signal_amplitude = 0b00000000;
	static uint32_t signal_duration = 0b000001111010000100100000; // Signal duration of 500 ms
	static uint32_t sample = 0b000000000000000000000000;
	static uint16_t frequency_set[8] =
	{
		697,
		770,
		825,
		941,
		1209,
		1336,
		1477,
		1633
	};
	// Table for quarter of sinus wave
	static uint8_t sinus_table[513] =
	{
		0b01111111,    // sample0
		0b10000001,    // sample1
		0b10000010,    // sample2
		0b10000100,    // sample3
		0b10000101,    // sample4
		0b10000111,    // sample5
		0b10001000,    // sample6
		0b10001010,    // sample7
		0b10001011,    // sample8
		0b10001101,    // sample9
		0b10001111,    // sample10
		0b10010000,    // sample11
		0b10010010,    // sample12
		0b10010011,    // sample13
		0b10010101,    // sample14
		0b10010110,    // sample15
		0b10011000,    // sample16
		0b10011001,    // sample17
		0b10011011,    // sample18
		0b10011100,    // sample19
		0b10011110,    // sample20
		0b10011111,    // sample21
		0b10100001,    // sample22
		0b10100010,    // sample23
		0b10100100,    // sample24
		0b10100101,    // sample25
		0b10100111,    // sample26
		0b10101000,    // sample27
		0b10101010,    // sample28
		0b10101011,    // sample29
		0b10101101,    // sample30
		0b10101110,    // sample31
		0b10110000,    // sample32
		0b10110001,    // sample33
		0b10110010,    // sample34
		0b10110100,    // sample35
		0b10110101,    // sample36
		0b10110111,    // sample37
		0b10111000,    // sample38
		0b10111001,    // sample39
		0b10111011,    // sample40
		0b10111100,    // sample41
		0b10111110,    // sample42
		0b10111111,    // sample43
		0b11000000,    // sample44
		0b11000010,    // sample45
		0b11000011,    // sample46
		0b11000100,    // sample47
		0b11000110,    // sample48
		0b11000111,    // sample49
		0b11001000,    // sample50
		0b11001001,    // sample51
		0b11001011,    // sample52
		0b11001100,    // sample53
		0b11001101,    // sample54
		0b11001110,    // sample55
		0b11010000,    // sample56
		0b11010001,    // sample57
		0b11010010,    // sample58
		0b11010011,    // sample59
		0b11010100,    // sample60
		0b11010101,    // sample61
		0b11010111,    // sample62
		0b11011000,    // sample63
		0b11011001,    // sample64
		0b11011010,    // sample65
		0b11011011,    // sample66
		0b11011100,    // sample67
		0b11011101,    // sample68
		0b11011110,    // sample69
		0b11011111,    // sample70
		0b11100000,    // sample71
		0b11100001,    // sample72
		0b11100010,    // sample73
		0b11100011,    // sample74
		0b11100100,    // sample75
		0b11100101,    // sample76
		0b11100110,    // sample77
		0b11100111,    // sample78
		0b11101000,    // sample79
		0b11101001,    // sample80
		0b11101001,    // sample81
		0b11101010,    // sample82
		0b11101011,    // sample83
		0b11101100,    // sample84
		0b11101101,    // sample85
		0b11101110,    // sample86
		0b11101110,    // sample87
		0b11101111,    // sample88
		0b11110000,    // sample89
		0b11110000,    // sample90
		0b11110001,    // sample91
		0b11110010,    // sample92
		0b11110010,    // sample93
		0b11110011,    // sample94
		0b11110100,    // sample95
		0b11110100,    // sample96
		0b11110101,    // sample97
		0b11110101,    // sample98
		0b11110110,    // sample99
		0b11110111,    // sample100
		0b11110111,    // sample101
		0b11111000,    // sample102
		0b11111000,    // sample103
		0b11111001,    // sample104
		0b11111001,    // sample105
		0b11111001,    // sample106
		0b11111010,    // sample107
		0b11111010,    // sample108
		0b11111011,    // sample109
		0b11111011,    // sample110
		0b11111011,    // sample111
		0b11111100,    // sample112
		0b11111100,    // sample113
		0b11111100,    // sample114
		0b11111100,    // sample115
		0b11111101,    // sample116
		0b11111101,    // sample117
		0b11111101,    // sample118
		0b11111101,    // sample119
		0b11111101,    // sample120
		0b11111110,    // sample121
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
		0b11111101,    // sample136
		0b11111101,    // sample137
		0b11111101,    // sample138
		0b11111101,    // sample139
		0b11111101,    // sample140
		0b11111100,    // sample141
		0b11111100,    // sample142
		0b11111100,    // sample143
		0b11111100,    // sample144
		0b11111011,    // sample145
		0b11111011,    // sample146
		0b11111011,    // sample147
		0b11111010,    // sample148
		0b11111010,    // sample149
		0b11111001,    // sample150
		0b11111001,    // sample151
		0b11111001,    // sample152
		0b11111000,    // sample153
		0b11111000,    // sample154
		0b11110111,    // sample155
		0b11110111,    // sample156
		0b11110110,    // sample157
		0b11110101,    // sample158
		0b11110101,    // sample159
		0b11110100,    // sample160
		0b11110100,    // sample161
		0b11110011,    // sample162
		0b11110010,    // sample163
		0b11110010,    // sample164
		0b11110001,    // sample165
		0b11110000,    // sample166
		0b11110000,    // sample167
		0b11101111,    // sample168
		0b11101110,    // sample169
		0b11101110,    // sample170
		0b11101101,    // sample171
		0b11101100,    // sample172
		0b11101011,    // sample173
		0b11101010,    // sample174
		0b11101001,    // sample175
		0b11101001,    // sample176
		0b11101000,    // sample177
		0b11100111,    // sample178
		0b11100110,    // sample179
		0b11100101,    // sample180
		0b11100100,    // sample181
		0b11100011,    // sample182
		0b11100010,    // sample183
		0b11100001,    // sample184
		0b11100000,    // sample185
		0b11011111,    // sample186
		0b11011110,    // sample187
		0b11011101,    // sample188
		0b11011100,    // sample189
		0b11011011,    // sample190
		0b11011010,    // sample191
		0b11011001,    // sample192
		0b11011000,    // sample193
		0b11010111,    // sample194
		0b11010101,    // sample195
		0b11010100,    // sample196
		0b11010011,    // sample197
		0b11010010,    // sample198
		0b11010001,    // sample199
		0b11010000,    // sample200
		0b11001110,    // sample201
		0b11001101,    // sample202
		0b11001100,    // sample203
		0b11001011,    // sample204
		0b11001001,    // sample205
		0b11001000,    // sample206
		0b11000111,    // sample207
		0b11000110,    // sample208
		0b11000100,    // sample209
		0b11000011,    // sample210
		0b11000010,    // sample211
		0b11000000,    // sample212
		0b10111111,    // sample213
		0b10111110,    // sample214
		0b10111100,    // sample215
		0b10111011,    // sample216
		0b10111001,    // sample217
		0b10111000,    // sample218
		0b10110111,    // sample219
		0b10110101,    // sample220
		0b10110100,    // sample221
		0b10110010,    // sample222
		0b10110001,    // sample223
		0b10110000,    // sample224
		0b10101110,    // sample225
		0b10101101,    // sample226
		0b10101011,    // sample227
		0b10101010,    // sample228
		0b10101000,    // sample229
		0b10100111,    // sample230
		0b10100101,    // sample231
		0b10100100,    // sample232
		0b10100010,    // sample233
		0b10100001,    // sample234
		0b10011111,    // sample235
		0b10011110,    // sample236
		0b10011100,    // sample237
		0b10011011,    // sample238
		0b10011001,    // sample239
		0b10011000,    // sample240
		0b10010110,    // sample241
		0b10010101,    // sample242
		0b10010011,    // sample243
		0b10010010,    // sample244
		0b10010000,    // sample245
		0b10001111,    // sample246
		0b10001101,    // sample247
		0b10001011,    // sample248
		0b10001010,    // sample249
		0b10001000,    // sample250
		0b10000111,    // sample251
		0b10000101,    // sample252
		0b10000100,    // sample253
		0b10000010,    // sample254
		0b10000001,    // sample255
		0b01111111,    // sample256
		0b01111101,    // sample257
		0b01111100,    // sample258
		0b01111010,    // sample259
		0b01111001,    // sample260
		0b01110111,    // sample261
		0b01110110,    // sample262
		0b01110100,    // sample263
		0b01110011,    // sample264
		0b01110001,    // sample265
		0b01101111,    // sample266
		0b01101110,    // sample267
		0b01101100,    // sample268
		0b01101011,    // sample269
		0b01101001,    // sample270
		0b01101000,    // sample271
		0b01100110,    // sample272
		0b01100101,    // sample273
		0b01100011,    // sample274
		0b01100010,    // sample275
		0b01100000,    // sample276
		0b01011111,    // sample277
		0b01011101,    // sample278
		0b01011100,    // sample279
		0b01011010,    // sample280
		0b01011001,    // sample281
		0b01010111,    // sample282
		0b01010110,    // sample283
		0b01010100,    // sample284
		0b01010011,    // sample285
		0b01010001,    // sample286
		0b01010000,    // sample287
		0b01001110,    // sample288
		0b01001101,    // sample289
		0b01001100,    // sample290
		0b01001010,    // sample291
		0b01001001,    // sample292
		0b01000111,    // sample293
		0b01000110,    // sample294
		0b01000101,    // sample295
		0b01000011,    // sample296
		0b01000010,    // sample297
		0b01000000,    // sample298
		0b00111111,    // sample299
		0b00111110,    // sample300
		0b00111100,    // sample301
		0b00111011,    // sample302
		0b00111010,    // sample303
		0b00111000,    // sample304
		0b00110111,    // sample305
		0b00110110,    // sample306
		0b00110101,    // sample307
		0b00110011,    // sample308
		0b00110010,    // sample309
		0b00110001,    // sample310
		0b00110000,    // sample311
		0b00101110,    // sample312
		0b00101101,    // sample313
		0b00101100,    // sample314
		0b00101011,    // sample315
		0b00101010,    // sample316
		0b00101001,    // sample317
		0b00100111,    // sample318
		0b00100110,    // sample319
		0b00100101,    // sample320
		0b00100100,    // sample321
		0b00100011,    // sample322
		0b00100010,    // sample323
		0b00100001,    // sample324
		0b00100000,    // sample325
		0b00011111,    // sample326
		0b00011110,    // sample327
		0b00011101,    // sample328
		0b00011100,    // sample329
		0b00011011,    // sample330
		0b00011010,    // sample331
		0b00011001,    // sample332
		0b00011000,    // sample333
		0b00010111,    // sample334
		0b00010110,    // sample335
		0b00010101,    // sample336
		0b00010101,    // sample337
		0b00010100,    // sample338
		0b00010011,    // sample339
		0b00010010,    // sample340
		0b00010001,    // sample341
		0b00010000,    // sample342
		0b00010000,    // sample343
		0b00001111,    // sample344
		0b00001110,    // sample345
		0b00001110,    // sample346
		0b00001101,    // sample347
		0b00001100,    // sample348
		0b00001100,    // sample349
		0b00001011,    // sample350
		0b00001010,    // sample351
		0b00001010,    // sample352
		0b00001001,    // sample353
		0b00001001,    // sample354
		0b00001000,    // sample355
		0b00000111,    // sample356
		0b00000111,    // sample357
		0b00000110,    // sample358
		0b00000110,    // sample359
		0b00000101,    // sample360
		0b00000101,    // sample361
		0b00000101,    // sample362
		0b00000100,    // sample363
		0b00000100,    // sample364
		0b00000011,    // sample365
		0b00000011,    // sample366
		0b00000011,    // sample367
		0b00000010,    // sample368
		0b00000010,    // sample369
		0b00000010,    // sample370
		0b00000010,    // sample371
		0b00000001,    // sample372
		0b00000001,    // sample373
		0b00000001,    // sample374
		0b00000001,    // sample375
		0b00000001,    // sample376
		0b00000000,    // sample377
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
		0b00000001,    // sample392
		0b00000001,    // sample393
		0b00000001,    // sample394
		0b00000001,    // sample395
		0b00000001,    // sample396
		0b00000010,    // sample397
		0b00000010,    // sample398
		0b00000010,    // sample399
		0b00000010,    // sample400
		0b00000011,    // sample401
		0b00000011,    // sample402
		0b00000011,    // sample403
		0b00000100,    // sample404
		0b00000100,    // sample405
		0b00000101,    // sample406
		0b00000101,    // sample407
		0b00000101,    // sample408
		0b00000110,    // sample409
		0b00000110,    // sample410
		0b00000111,    // sample411
		0b00000111,    // sample412
		0b00001000,    // sample413
		0b00001001,    // sample414
		0b00001001,    // sample415
		0b00001010,    // sample416
		0b00001010,    // sample417
		0b00001011,    // sample418
		0b00001100,    // sample419
		0b00001100,    // sample420
		0b00001101,    // sample421
		0b00001110,    // sample422
		0b00001110,    // sample423
		0b00001111,    // sample424
		0b00010000,    // sample425
		0b00010000,    // sample426
		0b00010001,    // sample427
		0b00010010,    // sample428
		0b00010011,    // sample429
		0b00010100,    // sample430
		0b00010101,    // sample431
		0b00010101,    // sample432
		0b00010110,    // sample433
		0b00010111,    // sample434
		0b00011000,    // sample435
		0b00011001,    // sample436
		0b00011010,    // sample437
		0b00011011,    // sample438
		0b00011100,    // sample439
		0b00011101,    // sample440
		0b00011110,    // sample441
		0b00011111,    // sample442
		0b00100000,    // sample443
		0b00100001,    // sample444
		0b00100010,    // sample445
		0b00100011,    // sample446
		0b00100100,    // sample447
		0b00100101,    // sample448
		0b00100110,    // sample449
		0b00100111,    // sample450
		0b00101001,    // sample451
		0b00101010,    // sample452
		0b00101011,    // sample453
		0b00101100,    // sample454
		0b00101101,    // sample455
		0b00101110,    // sample456
		0b00110000,    // sample457
		0b00110001,    // sample458
		0b00110010,    // sample459
		0b00110011,    // sample460
		0b00110101,    // sample461
		0b00110110,    // sample462
		0b00110111,    // sample463
		0b00111000,    // sample464
		0b00111010,    // sample465
		0b00111011,    // sample466
		0b00111100,    // sample467
		0b00111110,    // sample468
		0b00111111,    // sample469
		0b01000000,    // sample470
		0b01000010,    // sample471
		0b01000011,    // sample472
		0b01000101,    // sample473
		0b01000110,    // sample474
		0b01000111,    // sample475
		0b01001001,    // sample476
		0b01001010,    // sample477
		0b01001100,    // sample478
		0b01001101,    // sample479
		0b01001110,    // sample480
		0b01010000,    // sample481
		0b01010001,    // sample482
		0b01010011,    // sample483
		0b01010100,    // sample484
		0b01010110,    // sample485
		0b01010111,    // sample486
		0b01011001,    // sample487
		0b01011010,    // sample488
		0b01011100,    // sample489
		0b01011101,    // sample490
		0b01011111,    // sample491
		0b01100000,    // sample492
		0b01100010,    // sample493
		0b01100011,    // sample494
		0b01100101,    // sample495
		0b01100110,    // sample496
		0b01101000,    // sample497
		0b01101001,    // sample498
		0b01101011,    // sample499
		0b01101100,    // sample500
		0b01101110,    // sample501
		0b01101111,    // sample502
		0b01110001,    // sample503
		0b01110011,    // sample504
		0b01110100,    // sample505
		0b01110110,    // sample506
		0b01110111,    // sample507
		0b01111001,    // sample508
		0b01111010,    // sample509
		0b01111100,    // sample510
		0b01111101,    // sample511
		0b01111111,    // sample512


	};
	if(signal_duration >= sample)
	{
		signal_amplitude = (sinus_table[gen_sig_sample_id(sample, &frequency_set[key/4])] + sinus_table[gen_sig_sample_id(sample, &frequency_set[4 + (key % 4)])])/2;
		itoa(signal_amplitude, uart_message, 10);
		uart_puts(uart_message);
		sample++;
	}
	else
	{
		signal_duration = 0b00;
	}
	
	//uint8_t button;
	GPIO_toggle(&PORTC, PC0);

}



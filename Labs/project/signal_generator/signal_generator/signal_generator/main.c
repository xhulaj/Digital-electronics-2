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
#define COMP_REG_A_MASK 0b00010000;

#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "lcd.h"



int main(void)
{
	//############# TIMER/COUNTER0 SETTINGS
	TIM0_set_mode_CTC();			// Set timer to CTC mode
	TIM0_CTC_A_interrupt_enable();	// Interrupt enable
	OCR0A = COMP_REG_A_MASK;		// Set Compare register A mask for 1 MHz frequency
	
	// Enables interrupts by setting the global interrupt mask
	sei();
    while (1) 
    {
    }
}

ISR(TIMER0_COMPA_vect)
{
	static uint8_t sinus_table[359] =
	{
		0b	00000000	;	// sample 0
		0b	00000000	;	// sample 1
		0b	00000001	;	// sample 2
		0b	00000001	;	// sample 3
		0b	00000001	;	// sample 4
		0b	00000001	;	// sample 5
		0b	00000010	;	// sample 6
		0b	00000010	;	// sample 7
		0b	00000010	;	// sample 8
		0b	00000010	;	// sample 9
		0b	00000011	;	// sample 10
		0b	00000011	;	// sample 11
		0b	00000011	;	// sample 12
		0b	00000100	;	// sample 13
		0b	00000100	;	// sample 14
		0b	00000100	;	// sample 15
		0b	00000100	;	// sample 16
		0b	00000101	;	// sample 17
		0b	00000101	;	// sample 18
		0b	00000101	;	// sample 19
		0b	00000110	;	// sample 20
		0b	00000110	;	// sample 21
		0b	00000110	;	// sample 22
		0b	00000110	;	// sample 23
		0b	00000111	;	// sample 24
		0b	00000111	;	// sample 25
		0b	00000111	;	// sample 26
		0b	00000111	;	// sample 27
		0b	00001000	;	// sample 28
		0b	00001000	;	// sample 29
		0b	00001000	;	// sample 30
		0b	00001001	;	// sample 31
		0b	00001001	;	// sample 32
		0b	00001001	;	// sample 33
		0b	00001001	;	// sample 34
		0b	00001010	;	// sample 35
		0b	00001010	;	// sample 36
		0b	00001010	;	// sample 37
		0b	00001010	;	// sample 38
		0b	00001011	;	// sample 39
		0b	00001011	;	// sample 40
		0b	00001011	;	// sample 41
		0b	00001100	;	// sample 42
		0b	00001100	;	// sample 43
		0b	00001100	;	// sample 44
		0b	00001100	;	// sample 45
		0b	00001101	;	// sample 46
		0b	00001101	;	// sample 47
		0b	00001101	;	// sample 48
		0b	00001101	;	// sample 49
		0b	00001110	;	// sample 50
		0b	00001110	;	// sample 51
		0b	00001110	;	// sample 52
		0b	00001111	;	// sample 53
		0b	00001111	;	// sample 54
		0b	00001111	;	// sample 55
		0b	00001111	;	// sample 56
		0b	00010000	;	// sample 57
		0b	00010000	;	// sample 58
		0b	00010000	;	// sample 59
		0b	00010000	;	// sample 60
		0b	00010001	;	// sample 61
		0b	00010001	;	// sample 62
		0b	00010001	;	// sample 63
		0b	00010001	;	// sample 64
		0b	00010010	;	// sample 65
		0b	00010010	;	// sample 66
		0b	00010010	;	// sample 67
		0b	00010011	;	// sample 68
		0b	00010011	;	// sample 69
		0b	00010011	;	// sample 70
		0b	00010011	;	// sample 71
		0b	00010100	;	// sample 72
		0b	00010100	;	// sample 73
		0b	00010100	;	// sample 74
		0b	00010100	;	// sample 75
		0b	00010101	;	// sample 76
		0b	00010101	;	// sample 77
		0b	00010101	;	// sample 78
		0b	00010101	;	// sample 79
		0b	00010110	;	// sample 80
		0b	00010110	;	// sample 81
		0b	00010110	;	// sample 82
		0b	00010110	;	// sample 83
		0b	00010111	;	// sample 84
		0b	00010111	;	// sample 85
		0b	00010111	;	// sample 86
		0b	00010111	;	// sample 87
		0b	00011000	;	// sample 88
		0b	00011000	;	// sample 89
		0b	00011000	;	// sample 90
		0b	00011000	;	// sample 91
		0b	00011001	;	// sample 92
		0b	00011001	;	// sample 93
		0b	00011001	;	// sample 94
		0b	00011010	;	// sample 95
		0b	00011010	;	// sample 96
		0b	00011010	;	// sample 97
		0b	00011010	;	// sample 98
		0b	00011011	;	// sample 99
		0b	00011011	;	// sample 100
		0b	00011011	;	// sample 101
		0b	00011011	;	// sample 102
		0b	00011100	;	// sample 103
		0b	00011100	;	// sample 104
		0b	00011100	;	// sample 105
		0b	00011100	;	// sample 106
		0b	00011101	;	// sample 107
		0b	00011101	;	// sample 108
		0b	00011101	;	// sample 109
		0b	00011101	;	// sample 110
		0b	00011101	;	// sample 111
		0b	00011110	;	// sample 112
		0b	00011110	;	// sample 113
		0b	00011110	;	// sample 114
		0b	00011110	;	// sample 115
		0b	00011111	;	// sample 116
		0b	00011111	;	// sample 117
		0b	00011111	;	// sample 118
		0b	00011111	;	// sample 119
		0b	00100000	;	// sample 120
		0b	00100000	;	// sample 121
		0b	00100000	;	// sample 122
		0b	00100000	;	// sample 123
		0b	00100001	;	// sample 124
		0b	00100001	;	// sample 125
		0b	00100001	;	// sample 126
		0b	00100001	;	// sample 127
		0b	00100010	;	// sample 128
		0b	00100010	;	// sample 129
		0b	00100010	;	// sample 130
		0b	00100010	;	// sample 131
		0b	00100010	;	// sample 132
		0b	00100011	;	// sample 133
		0b	00100011	;	// sample 134
		0b	00100011	;	// sample 135
		0b	00100011	;	// sample 136
		0b	00100100	;	// sample 137
		0b	00100100	;	// sample 138
		0b	00100100	;	// sample 139
		0b	00100100	;	// sample 140
		0b	00100101	;	// sample 141
		0b	00100101	;	// sample 142
		0b	00100101	;	// sample 143
		0b	00100101	;	// sample 144
		0b	00100101	;	// sample 145
		0b	00100110	;	// sample 146
		0b	00100110	;	// sample 147
		0b	00100110	;	// sample 148
		0b	00100110	;	// sample 149
		0b	00100111	;	// sample 150
		0b	00100111	;	// sample 151
		0b	00100111	;	// sample 152
		0b	00100111	;	// sample 153
		0b	00100111	;	// sample 154
		0b	00101000	;	// sample 155
		0b	00101000	;	// sample 156
		0b	00101000	;	// sample 157
		0b	00101000	;	// sample 158
		0b	00101000	;	// sample 159
		0b	00101001	;	// sample 160
		0b	00101001	;	// sample 161
		0b	00101001	;	// sample 162
		0b	00101001	;	// sample 163
		0b	00101010	;	// sample 164
		0b	00101010	;	// sample 165
		0b	00101010	;	// sample 166
		0b	00101010	;	// sample 167
		0b	00101010	;	// sample 168
		0b	00101011	;	// sample 169
		0b	00101011	;	// sample 170
		0b	00101011	;	// sample 171
		0b	00101011	;	// sample 172
		0b	00101011	;	// sample 173
		0b	00101100	;	// sample 174
		0b	00101100	;	// sample 175
		0b	00101100	;	// sample 176
		0b	00101100	;	// sample 177
		0b	00101100	;	// sample 178
		0b	00101101	;	// sample 179
		0b	00101101	;	// sample 180
		0b	00101101	;	// sample 181
		0b	00101101	;	// sample 182
		0b	00101101	;	// sample 183
		0b	00101110	;	// sample 184
		0b	00101110	;	// sample 185
		0b	00101110	;	// sample 186
		0b	00101110	;	// sample 187
		0b	00101110	;	// sample 188
		0b	00101110	;	// sample 189
		0b	00101111	;	// sample 190
		0b	00101111	;	// sample 191
		0b	00101111	;	// sample 192
		0b	00101111	;	// sample 193
		0b	00101111	;	// sample 194
		0b	00110000	;	// sample 195
		0b	00110000	;	// sample 196
		0b	00110000	;	// sample 197
		0b	00110000	;	// sample 198
		0b	00110000	;	// sample 199
		0b	00110000	;	// sample 200
		0b	00110001	;	// sample 201
		0b	00110001	;	// sample 202
		0b	00110001	;	// sample 203
		0b	00110001	;	// sample 204
		0b	00110001	;	// sample 205
		0b	00110010	;	// sample 206
		0b	00110010	;	// sample 207
		0b	00110010	;	// sample 208
		0b	00110010	;	// sample 209
		0b	00110010	;	// sample 210
		0b	00110010	;	// sample 211
		0b	00110011	;	// sample 212
		0b	00110011	;	// sample 213
		0b	00110011	;	// sample 214
		0b	00110011	;	// sample 215
		0b	00110011	;	// sample 216
		0b	00110011	;	// sample 217
		0b	00110011	;	// sample 218
		0b	00110100	;	// sample 219
		0b	00110100	;	// sample 220
		0b	00110100	;	// sample 221
		0b	00110100	;	// sample 222
		0b	00110100	;	// sample 223
		0b	00110100	;	// sample 224
		0b	00110101	;	// sample 225
		0b	00110101	;	// sample 226
		0b	00110101	;	// sample 227
		0b	00110101	;	// sample 228
		0b	00110101	;	// sample 229
		0b	00110101	;	// sample 230
		0b	00110101	;	// sample 231
		0b	00110110	;	// sample 232
		0b	00110110	;	// sample 233
		0b	00110110	;	// sample 234
		0b	00110110	;	// sample 235
		0b	00110110	;	// sample 236
		0b	00110110	;	// sample 237
		0b	00110110	;	// sample 238
		0b	00110111	;	// sample 239
		0b	00110111	;	// sample 240
		0b	00110111	;	// sample 241
		0b	00110111	;	// sample 242
		0b	00110111	;	// sample 243
		0b	00110111	;	// sample 244
		0b	00110111	;	// sample 245
		0b	00111000	;	// sample 246
		0b	00111000	;	// sample 247
		0b	00111000	;	// sample 248
		0b	00111000	;	// sample 249
		0b	00111000	;	// sample 250
		0b	00111000	;	// sample 251
		0b	00111000	;	// sample 252
		0b	00111000	;	// sample 253
		0b	00111001	;	// sample 254
		0b	00111001	;	// sample 255
		0b	00111001	;	// sample 256
		0b	00111001	;	// sample 257
		0b	00111001	;	// sample 258
		0b	00111001	;	// sample 259
		0b	00111001	;	// sample 260
		0b	00111001	;	// sample 261
		0b	00111001	;	// sample 262
		0b	00111010	;	// sample 263
		0b	00111010	;	// sample 264
		0b	00111010	;	// sample 265
		0b	00111010	;	// sample 266
		0b	00111010	;	// sample 267
		0b	00111010	;	// sample 268
		0b	00111010	;	// sample 269
		0b	00111010	;	// sample 270
		0b	00111010	;	// sample 271
		0b	00111011	;	// sample 272
		0b	00111011	;	// sample 273
		0b	00111011	;	// sample 274
		0b	00111011	;	// sample 275
		0b	00111011	;	// sample 276
		0b	00111011	;	// sample 277
		0b	00111011	;	// sample 278
		0b	00111011	;	// sample 279
		0b	00111011	;	// sample 280
		0b	00111011	;	// sample 281
		0b	00111100	;	// sample 282
		0b	00111100	;	// sample 283
		0b	00111100	;	// sample 284
		0b	00111100	;	// sample 285
		0b	00111100	;	// sample 286
		0b	00111100	;	// sample 287
		0b	00111100	;	// sample 288
		0b	00111100	;	// sample 289
		0b	00111100	;	// sample 290
		0b	00111100	;	// sample 291
		0b	00111100	;	// sample 292
		0b	00111100	;	// sample 293
		0b	00111101	;	// sample 294
		0b	00111101	;	// sample 295
		0b	00111101	;	// sample 296
		0b	00111101	;	// sample 297
		0b	00111101	;	// sample 298
		0b	00111101	;	// sample 299
		0b	00111101	;	// sample 300
		0b	00111101	;	// sample 301
		0b	00111101	;	// sample 302
		0b	00111101	;	// sample 303
		0b	00111101	;	// sample 304
		0b	00111101	;	// sample 305
		0b	00111101	;	// sample 306
		0b	00111101	;	// sample 307
		0b	00111101	;	// sample 308
		0b	00111110	;	// sample 309
		0b	00111110	;	// sample 310
		0b	00111110	;	// sample 311
		0b	00111110	;	// sample 312
		0b	00111110	;	// sample 313
		0b	00111110	;	// sample 314
		0b	00111110	;	// sample 315
		0b	00111110	;	// sample 316
		0b	00111110	;	// sample 317
		0b	00111110	;	// sample 318
		0b	00111110	;	// sample 319
		0b	00111110	;	// sample 320
		0b	00111110	;	// sample 321
		0b	00111110	;	// sample 322
		0b	00111110	;	// sample 323
		0b	00111110	;	// sample 324
		0b	00111110	;	// sample 325
		0b	00111110	;	// sample 326
		0b	00111110	;	// sample 327
		0b	00111110	;	// sample 328
		0b	00111110	;	// sample 329
		0b	00111111	;	// sample 330
		0b	00111111	;	// sample 331
		0b	00111111	;	// sample 332
		0b	00111111	;	// sample 333
		0b	00111111	;	// sample 334
		0b	00111111	;	// sample 335
		0b	00111111	;	// sample 336
		0b	00111111	;	// sample 337
		0b	00111111	;	// sample 338
		0b	00111111	;	// sample 339
		0b	00111111	;	// sample 340
		0b	00111111	;	// sample 341
		0b	00111111	;	// sample 342
		0b	00111111	;	// sample 343
		0b	00111111	;	// sample 344
		0b	00111111	;	// sample 345
		0b	00111111	;	// sample 346
		0b	00111111	;	// sample 347
		0b	00111111	;	// sample 348
		0b	00111111	;	// sample 349
		0b	00111111	;	// sample 350
		0b	00111111	;	// sample 351
		0b	00111111	;	// sample 352
		0b	00111111	;	// sample 353
		0b	00111111	;	// sample 354
		0b	00111111	;	// sample 355
		0b	00111111	;	// sample 356
		0b	00111111	;	// sample 357
		0b	00111111		// sample 358
	};

}



# Lab 4: Interrupts, Timers

### Learning objectives

The purpose of the laboratory exercise is to understand the function of the interrupt, interrupt service routine, and the functionality of 
timer units. Another goal is to practice finding information in the MCU manual; specifically setting timer control registers.

## Table for given 5 prescaler values. 

| **Module** | **Number of bits** | **1** | **8** | **32** | **64** | **128** | **256** | **1024** |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| Timer/Counter0 | 8  | 16u | 128u | -- | 1.024m | -- | 4.096ms| 16.384m |
| Timer/Counter1 | 16 | 4.096m | 32.786m | -- | 262m | -- | 1.048 | 4.194 |
| Timer/Counter2 | 8  | 16u | 128u | 512u | 1.024m | 2.048m | 4.096m | 16.384m |

## Connection of LEDs and Push Buttons

| **LED/Button** | **PORT[pin]** |
| :-: | :-: |
| D1 | PB5[13] |
| D2 | PB4[12] |
| D3 | PB3[-11] |
| D4 | PB2[-10] |
| S1-A1 | PC1[A1] |
| S2-A2 | PC2[A2] |
| S3-A3 | PC3[A3] |

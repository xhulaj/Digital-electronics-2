# Lab 5: Display devices, 7-segment display

Binary values of the segments for display 0 to 9 on a common anode 7-segment display:

   | **Digit** | **A** | **B** | **C** | **D** | **E** | **F** | **G** | **DP** |
   | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
   | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 1 |
   | 1 | 1 | 0 | 0 |  1 | 1  |  1 | 1  | 1 |
   | 2 | 0  | 0  | 1  | 0  | 0  |  1 |  0 | 1 |
   | 3 | 0 | 0 | 0 | 0 | 1 | 1 | 0 | 1 |
   | 4 | 1  | 0  |  0 | 1  |  1 |  0 | 0  | 1 |
   | 5 |  0 |  1 |  0 | 0  | 1  | 0  |  0 | 1 |
   | 6 |  0 |  1 |  0 |  0 | 0  | 0  |  0| 1 |
   | 7 |  0 |  0 |  0 | 1  | 1 | 1  |  1 | 1 |
   | 8 | 0  | 0  |  0 | 0  |  0 | 0  | 0 | 1 |
   | 9 |  0 |  0 |  0 |  0 | 1  | 0 | 0  | 1 |
   
Connection of seven segment display:


| **7seg pin** | **shift register** | **register pin** |
| :-: | :-: | :-: |
| A.1 (12) | U2 | QA (15) |
| A.2 (9)  | U2 | QB (1)  |
| A.3 (8)  | U2 | QC (2)  |
| A.4 (6)  | U2 | QD (3)  |
|  a (11)  | U3 | QA (15) |
|  b (7)   | U3 | QB (1)  |
|  d (2)   | U3 | QD (3)  |
|  c (4)   | U3 | QC (2)  |
|  e (1)   | U3 | QE (4)  |
|  f (10)  | U3 | QF (5)  |
|  g (5)   | U3 | QG (6)  |
| DP (3)   | U3 | QH (7)  |

	
Purpose of two shift registers:
	
The purpose of shift registers 74HC595-U2 is to, based on clock timing, shift between consecutive 7-segment displays. Only one is lit at any  given moment, which is not perceivable to human eye. Human sight "refreshes" at much 
slower pace, thus creating the illusion, that all displays are lit at once. The other shift register, 74HC595-U3, gives the information of which segments are to be lit in order to
show desired symbol, refreshed at the same frequency as aforementioned register.
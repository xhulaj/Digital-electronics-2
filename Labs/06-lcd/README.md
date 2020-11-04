# Lab 6: Display devices, LCD display

## Connection of LCD pins and meaning of affiliated signals

  | **LCD signal(s)** | **AVR pin(s)** | **Description** |
   | :-: | :-: | :-- |
   | RS | PB0 | Register selection signal. Selection between Instruction register (RS=0) and Data register (RS=1) |
   | R/W | GND | Read/Write selection |
   | E | PB1 | Enable |
   | D[3:0] | Not connected | Lower nibble for data byte |
   | D[7:4] | PD[7-4] | Higher nibble for data byte |
## ASCII
ASCII is American Standard Code for Information Interchange. It is a table treating english characters and other mostly used characters.
Values for uppercase 'A' to 'Z' are 0x40 to 0x5a, 'a' to 'z' are 0x61 to 0x7a, '0' to '9' are 0x30 to 0x39.

## Input parameters and description of the functions

   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `lcd_init` | `LCD_DISP_OFF`<br>`LCD_DISP_ON`<br>`LCD_DISP_ON_CURSOR`<br>`LCD_DISP_ON_CURSOR_BLINK` | Display off&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br>&nbsp;<br>&nbsp;<br>&nbsp; | `lcd_init(LCD_DISP_OFF);`<br>&nbsp;<br>&nbsp;<br>&nbsp; |
   | `lcd_clrscr` | `VOID`| Clear display and set cursor to home position | `lcd_clrscr();` |
   | `lcd_gotoxy` | `x`(horizontal position)<br>`y(vertical position)` | Set cursor to specified position &nbsp;<br>&nbsp;| `lcd_gotoxy();`&nbsp;<br>&nbsp; |
   | `lcd_putc` | `c`(single character) | Display character at current cursor position | `lcd_putc('k');` |
   | `lcd_puts` | `a`(string) | Display string from program memory without auto linefeed | `lcd_puts(string_variable);` |
   | `lcd_command` | `cmd`(instruction to send to LCD controller | Send data byte to LCD controller | `lcd_command(1 << LCD_DDRAM);` |
   | `lcd_data` | `data`(byte to send to LCD controller) | Send data byte to LCD controller | `lcd_data(customChar[i]);` |

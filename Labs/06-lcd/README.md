# Lab 6: Display devices, LCD display

## Connection of LCD pins and meaning of affiliated signals

  | **LCD signal(s)** | **AVR pin(s)** | **Description** |
   | :-: | :-: | :-- |
   | RS | PB0 | Register selection signal. Selection between Instruction register (RS=0) and Data register (RS=1) |
   | R/W | GND | Ground pin |
   | E | PB1 | Not sure |
   | D[3:0] | Not connected | Lower nibble for data byte |
   | D[7:4] | PD[7-4] | Higher nibble for data byte |
## ASCII
ASCII is American Standard Code for Information Interchange. It is a table treating english characters and other mostly used characters.
Values for uppercase 'A' to 'Z' are 0x40 to 0x5a, 'a' to 'z' are 0x61 to 0x7a, '0' to '9' are 0x30 to 0x39.
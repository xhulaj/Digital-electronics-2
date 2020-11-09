# Lab 7: ADC, serial communication, UART

## Lab prerequisites

### Connection of five push buttons: Select, Left, Up, Down, and Right on [LCD keypad shield](../../Docs/arduino_shield.pdf)
    All buttons have common ground and are connected to a signle pin through voltage divider. Which button is pressed is distinguished by the voltage on the output pin.

### Voltage value on pin PC0[A0] if one button is pressed at a time

 | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** |
   | :-: | :-: | :-: | :-: |
   | Right  | 0&nbsp;V | 0   |  |
   | Up     | 0.495&nbsp;V | 101 |  |
   | Down   | 1.203&nbsp;V | 246 |  |
   | Left   | 1.969&nbsp;V | 402 |  |
   | Select | 3.182&nbsp;V | 651 |  |
   | none   | 5&nbsp;V | 1023 |  |

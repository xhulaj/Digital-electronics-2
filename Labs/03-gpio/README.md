# Lab 3: Creation of user library for GPIO control

#### Contents

1. [Preparation tasks](#Preparation-tasks)


## Preparation tasks

Table of bits and numeric range for the selected data types defined by C.

| **Data type** | **Number of bits** | **Range** | **Description** |
| :-: | :-: | :-: | :-- | 
| `uint8_t`  | 8 | 0, 1, ..., 255 | Unsigned 8-bit integer |
| `int8_t`   | 8 | integers -127 to 127 | Signed 8-bit integer, MSb defines polarity |
| `uint16_t` | 16 | integers 0 to 65 535 | Unsigned 16-bit integer |
| `int16_t`  | 16 | integers -32 768 to 32 767 | Signed 16-bit integer, MSb defines polarity |
| `float`    | 32 | -3.4e+38, ..., 3.4e+38 | Single-precision floating-point |
| `void`     | 0 | none | Function return type |

Completed code:

```makefile
#include <avr/io.h>

uint16_t calculate(uint8_t,uint8_t);

int main(void)
{
    uint8_t a = 156;
    uint8_t b = 14;
    uint16_t c;

    c = calculate(a, b);

    while (1)
    {
    }
    return 0;
}

uint16_t calculate(uint8_t x, uint8_t y)
{
    uint16_t result;    // result = x^2 + 2xy + y^2

    result = x*x;
    result += 2 * x * y;
	result += y * y;
    return result;
}

```
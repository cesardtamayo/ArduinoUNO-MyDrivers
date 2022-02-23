#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

void UART_Init(void);
void serialWrite(char c[]);
// void serialWrite(int intInput);
char getChar(void);

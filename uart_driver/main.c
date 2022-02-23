/*
 */

#include "uart.h"
#include <util/delay.h>

int main(void)
{

    // Insert code
    UART_Init();

    _delay_ms(5000); // giving time to open serial port ...


    serialWrite("Testing serial port ...\n");

    serialWrite("Single-digit integer:\n");
    serialWrite("4");
    serialWrite("\n");

    serialWrite("Double-digit integer:\n");
    serialWrite("2314");
    serialWrite("\n");


    while(1){

    }

    return 0;
}

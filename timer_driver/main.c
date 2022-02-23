/*
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include <util/delay.h>


/*
Author: Cesar Tamayo

Glosary:
TCCR = Timer/Counter Control Register
WGM2:0 = Waveform Gneration Mode bits
CS02:0 = Clock Select bits
TIMSKn = Timer/Counter Interrupt Mask Register
OCIEnA = Timer/Counter n Output Compare Match A Interrupt Enable
OCRA = Output Compare Register A

*/

int extraTime = 0;

typedef struct {
    int mode;    int prescaler;
    int ticksCount
}avr_timer_8b;

avr_timer_8b new_avr_timer_8b(int modeValue, int prescalerValue, int ticksCountValue){
    avr_timer_8b result = {
        .mode = modeValue,
        .prescaler = prescalerValue,
        .ticksCount = ticksCountValue};
    return result;
}

ISR(TIMER0_COMPA_vect){
    extraTime++;

    if (extraTime > 100){
        PORTB ^= _BV(PORTB5); // toggle LED pin
        extraTime = 0;
    }
}

int avr_timer_8b_Init(avr_timer_8b timer){
    UART_Init();

    // mode configuration:
    switch(timer.mode){
    case 0:
        break;
    case 1:
        TCCR0A = (1<<WGM00);
        break;
    case 2:
        TCCR0A = (1<<WGM01);            // CTC (clear time on compare match) mode
        break;
    case 3:
        TCCR0A = (1<<WGM01) | (1<<WGM00);
        break;
    // PENDING OTHER MODES...
    default:
        serialWrite("Invalid timer mode input. Valid inputs are: normal, pwm1, ctc, fast pwm, reserved, pwm2.");
        return -1;
    }


    OCR0A = timer.ticksCount;       // configuring number of ticks to count
    TIMSK0 = (1<<OCIE0A);           // Enabling interrupt for match A (OCR0A)

    // prescaler configuration:
    switch(timer.prescaler){
        case 0:
            TCCR0B = (1<<CS00);             // clk/0 - no prescaling
            break;
        case 8:
            TCCR0B = (1<<CS01);             // clk/8
            break;
        case 64:
            TCCR0B = (1<<CS01) | (1<<CS00); // clk/64
            break;
        case 256:
            TCCR0B = (1<<CS02);             // clk/256
            break;
        case 1024:
            TCCR0B = (1<<CS02) | (1<<CS00); // clk/1024
            break;
        default:
            serialWrite("Invalid prescaler input: value must be 0,8,64,256 or 1024.");
            return -1;
    }

    sei(); // enabling interrupts
    serialWrite("Timer initialized successfully.");
    return 0;
}


int main(void)
{
    UART_Init();
    _delay_ms(5000);     // giving time to open Serial Port ...

    serialWrite("Creating timer object ...\n");
    avr_timer_8b myTimer2 = new_avr_timer_8b(2,1024,156);

    if (strcmp(myTimer2.mode,"test") != 0 ){
        serialWrite("mode match\n");
    }
    if (myTimer2.prescaler == 1024){
        serialWrite("prescaler match\n");
    }
    if (myTimer2.ticksCount == 195){
        serialWrite("ticksCount match\n");
    }


    /*
    avr_timer myTimer;
    myTimer.bitSize = 8;
    // myTimer.mode = 'test';
    myTimer.prescaler = 1024;
    myTimer.ticksCount = 195;

    serialWrite("timer details:\n");
    serialWrite(12345);
    serialWrite(myTimer.bitSize);
    serialWrite(myTimer.mode);
    serialWrite(myTimer.prescaler);
    serialWrite(myTimer.ticksCount);

    */

    DDRB |= _BV(DDB5);    // setting LED pin to output
    avr_timer_8b_Init(myTimer2);

    while(1){
    }

    return 0;
}


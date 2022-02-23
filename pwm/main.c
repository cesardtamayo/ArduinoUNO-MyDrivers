/*
Glosary:
TCCR = Timer/Counter Control Register
WGM2:0 = Waveform Gneration Mode bits
CS02:0 = Clock Select bits
TIMSKn = Timer/Counter Interrupt Mask Register
OCIEnA = Timer/Counter n Output Compare Match A Interrupt Enable
OCRA = Output Compare Register A

 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int dutyCycle = 0;  // 0-255
int stepSize = 10;

ISR(TIMER0_OVF_vect){
    OCR0A = dutyCycle;     // Duty Cycle set
}

int main(void)
{

    DDRD = (1<<PORTD6); // Enabling output pin for LED

    // Configuring Fast PWM:
    TCCR0A = (1<<COM0A1) | (1<<WGM01) | (1<<WGM00); // Clear OC0A on Compare Match and Waveform Generation Mode = Fast PWM
    TIMSK0 = (1<<TOIE0);    // Timer/Counter Overflow Interrupt Enable
    OCR0A = dutyCycle;     // Duty Cycle config

    sei();

    TCCR0B = (1<<CS00) | (1<<CS02); // Prescaler = 1 - TIMER STARTS WORKING

    while(1){

        _delay_ms(50);

        dutyCycle = dutyCycle + stepSize;
        if (dutyCycle > 240){
            stepSize *= (-1);  // inverting direction of dutyCycle change
        }else if (dutyCycle < 10){
            stepSize *= (-1);  // inverting direction of dutyCycle change
        }

    }

    return 0;
}

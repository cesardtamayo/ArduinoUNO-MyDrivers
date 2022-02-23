/*
Author: ctamayo

Glosary:
MSPIM = Master SPI Mode.
UBRR = Universal Baud Rate Register.
XCK = Transfer Clock.
UCSRnA = USART Control and Status Register n A.
UCSRnB = USART Control and Status Register n B.
UCSRnC = USART Control and Status Register n C.
RXENn = Receiver Enable n.
TXENn = Transmitter Enable n.
TXCIEn = TX Complete Interrupt Enable n.
UCSZn2:0 = Sets number of data bits (character size) in a frame.
RXCn = Receive Complete.
TXCn = Transmit Complete.
UDREn = Data Register Empty.

*/

#include "uart.h"

#define F_CPU   16000000
#define BAUD    9600
#define BRC     ((F_CPU/16/BAUD)-1)
#define TX_BUFFER_SIZE 128
#define RX_BUFFER_SIZE 128

char txBuffer[TX_BUFFER_SIZE];
uint8_t txReadPos = 0;
uint8_t txWritePos = 0;
char rxBuffer[RX_BUFFER_SIZE];
uint8_t rxReadPos = 0;
uint8_t rxWritePos = 0;

void UART_Init(void){
    // Configure Baud rate:
    UBRR0H = (BRC>>8);
    UBRR0L = BRC;
    // Enable receiver and transmitter:
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    // Enable Rx/Tx interrupts:
    UCSR0B |= (1<<TXCIE0);
    UCSR0B |= (1<<RXCIE0);
    sei();

    // Configure Character Size:
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
    // Stop bits = 1 by default
}

int UART_Read(void){
    // Wait for data to be received
    while (!(UCSR0A & (1<<RXC0)));
    // Return received data from buffer
    return UDR0;
}

int UART_Write(int data){
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1<<UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = data;
    return data;
}

void appendSerialTx(char c){
    txBuffer[txWritePos] = c;
    txWritePos++;
    if (txWritePos >= TX_BUFFER_SIZE)
        txWritePos = 0;
}

ISR(USART_TX_vect){
    if (txReadPos != txWritePos){
        UDR0 = txBuffer[txReadPos];
        txReadPos++;

        if (txReadPos >= TX_BUFFER_SIZE)
            txReadPos = 0;
    }
}

void serialWrite(char c[]){
    uint8_t i;
    for (i = 0; i < strlen(c); i++ ){
        appendSerialTx(c[i]);
    }

    if (UCSR0A & (1 <<UDRE0)){
        UDR0 = 0;
    }
}

ISR(USAR_RX_vect){
    rxBuffer[rxWritePos] = UDR0;
    rxWritePos++;

    if(rxWritePos >= RX_BUFFER_SIZE)
        rxWritePos = 0;
}

char getChar(void){
    char bufferChar = '\0';

    if(rxReadPos != rxWritePos){
        bufferChar = rxBuffer[rxReadPos];
        rxReadPos++;

        if (rxReadPos >= RX_BUFFER_SIZE)
            rxReadPos = 0;
    }
}

char peekChar(void){
}





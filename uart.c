#include <avr/io.h>
#include "uart.h"

void SerialInit(unsigned long int fosc, unsigned int baud, short int bits, short int stopBits, short int parity)
{
	unsigned int ubrr = ((fosc+baud*8UL) / (16UL*baud)-1);

	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;

	UCSR1B = (1<<RXEN1)|(1<<TXEN1);

	switch(parity)
	{
		case 0:
			UCSR1C = (0<<UPM10) | (0<<UPM11);
			break;
			
		case 1:
			UCSR1C = (1<<UPM11);
			break;
			
		case 2:
			UCSR1C = (1<<UPM11) | (1<<UPM10);
			break;
	}

	switch(stopBits)
	{
		case 1:
			UCSR1C = (0<<USBS1);
			break;
			
		case 2:
			UCSR1C = (1<<USBS1);
			break;
	}

	switch(bits)
	{
		case 6:
			UCSR1C = (1<<UCSZ10);
			break;
			
		case 7:
			UCSR1C = (1<<UCSZ11);
			break;
			
		case 8:
			UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
			break;
	}
}

void SerialTransmitChar(unsigned char data)
{
	while (!(UCSR1A & (1<<UDRE1)));

	UDR1 = data;
}

void SerialTransmit(unsigned char s[])
{
	unsigned char i;
	for (i = 0; i < 16; i++){
		if( s[i] == 0){
			s[i] 	= 13;	// CR
			s[i+1] 	= 10;	// LF
			s[i+2] 	= 0;	// end
			break;
		}
	}
	
	unsigned char k = 0;
	while(s[k] != 0)
	{
		SerialTransmitChar(s[k]);
		k++;
	}
}

unsigned char SerialReceive(void)
{
	while (!(UCSR1A & (1<<RXC1)));
	return UDR1;
}
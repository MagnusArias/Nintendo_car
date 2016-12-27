#include <avr/io.h>
#include "uart.h"

#define RX_BUFFER_IS_FULL() ((UCSR0A & (1 << RXC0)) == 0)
#define TX_BUFFER_IS_FULL() ((UCSR0A & (1 << UDRE0)) == 0)

void SerialInit(unsigned long int fosc, unsigned long int baud, unsigned char speed, short int bits, short int stopBits, short int parity)
{
	UCSR0A |= (1<<U2X0);
	unsigned int ubrr = 12;
		
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B |= (1<<TXEN0)|(1<<RXEN0);
	
	switch(parity)
	{
		case 0:
		UCSR0C |= (0<<UPM00) | (0<<UPM01);
		break;
		
		case 1:
		UCSR0C |= (1<<UPM01);
		break;
		
		case 2:
		UCSR0C |= (1<<UPM01) | (1<<UPM00);
		break;
	}

	switch(stopBits)
	{
		case 1:
		UCSR0C |= (0<<USBS0);
		break;
		
		case 2:
		UCSR0C |= (1<<USBS0);
		break;
	}

	switch(bits)
	{
		case 6:
		UCSR0C |= (1<<UCSZ00);
		break;
		
		case 7:
		UCSR0C |= (1<<UCSZ01);
		break;
		
		case 8:
		UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
		break;
	}
}

void SerialTransmitChar(unsigned char data)
{
	while (TX_BUFFER_IS_FULL());

	UDR0 = data;
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
	while(s[k] != '\0')	SerialTransmitChar(s[k++]);
}

unsigned char SerialReceiveChar(void)
{
	while (RX_BUFFER_IS_FULL());
	return UDR0;
}

unsigned char SerialReceive(unsigned char *dest, unsigned char size)
{
	unsigned char i = 0;
	if (size == 0) return 0;
	
	while (i < size - 1)
	{
		unsigned char c;
		c = SerialReceiveChar();
		
		if (c == '\0')
		{
			break;
		}
		dest[i] = c;
		i++;
	}
	dest[i] = 0;
	
	return i + 1;
}

void SerialFlush(void)
{
	unsigned char dummy;
	while (UCSR0A & (1<<RXC0)) dummy = UDR0;
}
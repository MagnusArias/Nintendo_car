#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "uart.h"

#define RX_BUFFER_IS_FULL() ((UCSR0A & (1 << RXC0)) == 0)
#define TX_BUFFER_IS_FULL() ((UCSR0A & (1 << UDRE0)) == 0)

void SerialInit(void)
{
	UCSR0A |= (1<<U2X0);
	unsigned int ubrr = 19;	
	/*
		Wartoœæ dla oscylatora 18.432MHz i baudrate 115200, wartoœæ b³edu wynosi +0.0%
			Tê wartoœæ nale¿y ustawiæ rêcznie po wyliczeniu jej przy pomocy
					http://wormfood.net/avrbaudcalc.php
	*/
	
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B |=	(1<<TXEN0) | (1<<RXEN0);
	
	UCSR0C |=	(0<<UPM00) | (0<<UPM01) |	// no parity
				(0<<USBS0) |				// 1 stop bit
				(1<<UCSZ01)| (1<<UCSZ00);	// 8 data bits
}

void SerialTransmitChar(unsigned char data)
{
	while (TX_BUFFER_IS_FULL());
	UDR0 = data;
}

void SerialTransmit(unsigned char s[])
{
	uint8_t len = strlen(s);
	s[len] 	 = 13;	// CR
	s[len+1] = 10;	// LF
	s[len+2] = 0;	// end
			
	unsigned char k = 0;
	while(s[k] != '\0') SerialTransmitChar(s[k++]);
	_delay_ms(5);
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
		if (c == '\0') break;
		
		dest[i++] = c;
	}
	dest[i] = 0;
	return i + 1;
}

void SerialFlush(void)
{
	unsigned char dummy;
	while (UCSR0A & (1<<RXC0)) dummy = UDR0;
}

void SendStruct( struct sDataPacket *src )
{
	char* srcPointer = src;

	char size = sizeof(*src);

	char i;
	for (i = 0; i < size; i++)
	{
		SerialTransmitChar(*srcPointer++);
	}
}

void ReceiveStruct( struct sControlPacket *dst, uint8_t size )
{
	char* dstPointer = dst;

	char i;
	for (i = 0; i < size; i++)
	{
		*dstPointer = SerialReceiveChar();
		dstPointer++;
	}
}
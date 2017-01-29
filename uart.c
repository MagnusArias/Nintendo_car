#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "uart.h"

#define RX_BUFFER_IS_FULL() ((UCSR0A & (1 << RXC0)) == 0)
#define TX_BUFFER_IS_FULL() ((UCSR0A & (1 << UDRE0)) == 0)

void SerialInit(void)
{
	/*
		Initializes one of the UARTs from AVR. To connect to the ESP8266 we have to:
			 - baud rate equal to 115200
			 - 8 data bits
			 - no parity
			 - 1 stop bit
	
		Value UBRR adjusted for crystal oscillator 18.432MHz and baud rate 115200, error equals 0.0%
		This value has to be set manually after calculating it using this page:
		http://wormfood.net/avrbaudcalc.php
	*/
	
	UCSR0A |= (1<<U2X0);
	unsigned int ubrr = 19;	
		
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B |=	(1<<TXEN0) | (1<<RXEN0);
	
	UCSR0C |=	(0<<UPM00) | (0<<UPM01) |	// no parity
				(0<<USBS0) |				// 1 stop bit
				(1<<UCSZ01)| (1<<UCSZ00);	// 8 data bits
}

void SerialTransmitChar(unsigned char data)
{
	/*
		Transmits single character
	*/
	
	while (TX_BUFFER_IS_FULL());
	UDR0 = data;
}

void SerialTransmit(unsigned char s[])
{
	/*
		Transmits the whole string, adding necessary CR+LF at the end
	*/
	
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
	/*
		Receives and returns single character
	*/
	
	while (RX_BUFFER_IS_FULL());
	return UDR0;
}

unsigned char SerialReceive(unsigned char *dest, unsigned char size)
{
	/*
		Receives size-long string and returns the real size of received data
		Always waits for "size" to receive.
		If you want to receive 20 bytes data, but you know your data will be greater than 20 bytes,
		it will receive 20 bytes of data, but the rest will be lost.

		If you want to receive 20 bytes data, but you know your data will be less than 20 bytes,
		it will wait for another 5 bytes to receive...
		 
		... and I don't fucking know why, even with breaking the loop.
	*/
	
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
	/*
		Flushes the buffer, obviously
	*/
	
	unsigned char dummy;
	while (UCSR0A & (1<<RXC0)) dummy = UDR0;
}

void SendStruct( struct sDataPacket *src )
{
	/*
		Sends filled structure to 3DS, char by char
		that's why I had to add 3 fields (CR+LF+0) to structure 
	*/
	
	char* srcPointer = src;

	char size = sizeof(*src);

	char i;
	for (i = 0; i < size; i++)
	{
		SerialTransmitChar(*srcPointer++);
	}
}

void ReceiveStruct( struct sControlPacket *dst, unsigned char* src, uint8_t size )
{
	char* dstPointer = dst;

	char i;
	for (i = 0; i < size; i++)
	{
		*dstPointer = src;
		dstPointer++;
	}
}
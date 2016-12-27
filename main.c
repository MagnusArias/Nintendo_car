#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main(void)
{
	char rx[8];
	SerialInit(12000000, 115200, 2, 8, 1, 0);
	/* 
	uzyto kwarcu 12.000MHz:	-U lfuse:w:0xD7:m 
	predkosc U2X = 1, gdyz daje to mniejsze wartosci bledu (0.2%)
	8 bitow danych
	1 bit stopu
	brak parzystosci
	*/
	
	SerialTransmit("AT+GMR");
	while(1)
	{
		SerialReceive(rx, 8);
		// w wyniku zwraca 'Ai-Thinker Techn...
		//					016 11:29:20;K v...'
				
	}  
}
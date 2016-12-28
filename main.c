#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
#include "uart.h"

int main(void)
{
	LCD_Initalize();
	_delay_ms(1000);
	LCD_Clear();
	
	
	SerialInit(8, 1, 0);
	SerialTransmit("AT+CIPSERVER=1");
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
		unsigned char rx[20];
		SerialReceive(rx, 20);
		LCD_Clear();
		LCD_WriteText(rx);
		_delay_ms(750);
	} 
		SerialReceive(rx, 8);
		// w wyniku zwraca 'Ai-Thinker Techn...
		//					016 11:29:20;K v...'
				
	}  

}


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
	SerialTransmit("AT"); // przykladowa komenda do testu komunikacji
	/* 
	uzyto kwarcu 12.000MHz:	-U lfuse:w:0xD7:m 
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
}
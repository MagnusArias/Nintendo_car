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
	
	while(1)
	{
		unsigned char rx[20];
		SerialReceive(rx, 20);
		LCD_Clear();
		LCD_WriteText(rx);
		_delay_ms(750);
	} 
}
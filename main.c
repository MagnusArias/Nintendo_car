#define F_CPU 16000000UL
#include <avr/io.h> 
#include <util/delay.h>  
 
#include "HD44780.h"
#define FOSC F_CPU
#define BAUD 115200

int main(void)  
{  	
	LCD_Initalize();
	while(1)
	{	
		LCD_Clear();
		LCD_GoTo(0,0);
		LCD_WriteText("Hello!");
	}
	
	return 0; 
} 


#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
#include "i2c.h"
#include "lsm303d.h"

int main(void)
{
	LCD_Initalize();
	I2C_SetBitrate(100);
	LSM303D_Init_I2C();
	
	int bufor[2];
	int wynikX, wynikY, wynikZ;


	while(1)
	{
		TWI_read_buf( LSM303D_ADDR, OUT_X_H_A , 8, bufor );
		wynikX = (( (bufor[1])<<8) | (bufor[0]));
		
		TWI_read_buf( LSM303D_ADDR, OUT_Y_H_A , 8, bufor );
		wynikY = (( (bufor[1])<<8) | (bufor[0]));

		TWI_read_buf( LSM303D_ADDR, OUT_Z_H_A , 8, bufor );
		wynikZ = (( (bufor[1])<<8) | (bufor[0]));
		
		LCD_GoTo(0,0);	LCD_WriteText("X:   Y:   Z:");
		
		LCD_GoTo(0,1);	LCD_WriteInteger(wynikX);
		
		LCD_GoTo(5,1);	LCD_WriteInteger(wynikY);
		
		LCD_GoTo(10,1);	LCD_WriteInteger(wynikZ);
		
		_delay_ms(50);
		LCD_Clear();
	} 
	
}
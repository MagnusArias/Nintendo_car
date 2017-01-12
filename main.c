#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
#include "i2c.h"
#include "uart.h"
#include "lsm303d.h"


int main(void)
{	
	unsigned char mux[]		= "AT+CIPMUX=1";
	unsigned char server[]	= "AT+CIPSERVER=1";
	unsigned char send[]	= "AT+CIPSEND=0,17";
	
	unsigned char mode[]	= "AT+CWMODE=1";
	unsigned char ap[]		= "AT+CWJAP=\"Shin\",\"haslo12345\"";
	unsigned char cif[]		= "AT+CIFSR";
	
	uint8_t bufor[2];
	
	struct sDataPacket sData;
	
	LCD_Initalize();
	_delay_ms(1000);
	LCD_Clear();
	unsigned char rx[40];
	I2C_SetBitrate(100);
	LSM303D_Init_I2C();
	
	sData.__CR = 0x0D;
	sData.__LF = 0x0A;
	sData.__0 = 0;
	
	SerialInit();
	//SerialTransmit(mux);	LCD_WriteText("mux");	SerialReceive(rx, 20);	LCD_WriteText(rx);	_delay_ms(2000);	LCD_Clear();
	SerialTransmit(mode);	LCD_WriteText("mode ");	//SerialReceive(rx, 20);	LCD_WriteText(rx);	_delay_ms(2000);	LCD_Clear();
	SerialTransmit(ap);		LCD_WriteText("ap ");	//SerialReceive(rx, 20);	LCD_WriteText(rx);	_delay_ms(2000);	LCD_Clear();
	SerialTransmit(server);	LCD_WriteText("server ");//SerialReceive(rx, 20);	LCD_WriteText(rx);	_delay_ms(2000);	LCD_Clear();
	//SerialTransmit(cif);	LCD_WriteText("cif");	SerialReceive(rx, 18);	LCD_WriteText(rx);	_delay_ms(2000);	LCD_Clear();
	
	LCD_WriteText("\nPodlaczono!");
	_delay_ms(2000);
	LCD_Clear();
		
	while(1)
	{
		TWI_read_buf(LSM303D_ADDR,	OUT_X_H_A,	2, bufor);	sData._accX = (bufor[1] << 8 | bufor[0]);
		TWI_read_buf(LSM303D_ADDR,	OUT_Y_H_A,	2, bufor);	sData._accY = (bufor[1] << 8 | bufor[0]);
		TWI_read_buf(LSM303D_ADDR,	OUT_Z_H_A,	2, bufor);	sData._accZ = (bufor[1] << 8 | bufor[0]);
		TWI_read_buf(LSM303D_ADDR,	TEMP_OUT_H, 2, bufor);	sData._temp = (bufor[1] << 8 | bufor[0]);
		TWI_read_buf(LSM303D_ADDR,	OUT_X_H_M,	2, bufor);	sData._magX = (bufor[1] << 8 | bufor[0]);
		TWI_read_buf(LSM303D_ADDR,	OUT_Y_H_M,	2, bufor);	sData._magY = (bufor[1] << 8 | bufor[0]);
		TWI_read_buf(LSM303D_ADDR,	OUT_Z_H_M,	2, bufor);	sData._magZ = (bufor[1] << 8 | bufor[0]);
				
		SendStruct(&sData);
		_delay_ms(50);			
		SerialTransmit(send);
	} 
}
#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
#include "i2c.h"
#include "uart.h"
#include "lsm303d.h"

void readSensors(struct sDataPacket s)
{
	/*
		Read data from LSM303D, from specified sensors
		then saves content of buffer to given element of structure
	*/
	
	uint8_t buffer[2];
	
	TWI_read_buf(LSM303D_ADDR,	OUT_X_H_A,	2, buffer);	s._accX = (buffer[1] << 8 | buffer[0]);
	TWI_read_buf(LSM303D_ADDR,	OUT_Y_H_A,	2, buffer);	s._accY = (buffer[1] << 8 | buffer[0]);
	TWI_read_buf(LSM303D_ADDR,	OUT_Z_H_A,	2, buffer);	s._accZ = (buffer[1] << 8 | buffer[0]);
	TWI_read_buf(LSM303D_ADDR,	TEMP_OUT_H, 2, buffer);	s._temp = (buffer[1] << 8 | buffer[0]);
	TWI_read_buf(LSM303D_ADDR,	OUT_X_H_M,	2, buffer);	s._magX = (buffer[1] << 8 | buffer[0]);
	TWI_read_buf(LSM303D_ADDR,	OUT_Y_H_M,	2, buffer);	s._magY = (buffer[1] << 8 | buffer[0]);
	TWI_read_buf(LSM303D_ADDR,	OUT_Z_H_M,	2, buffer);	s._magZ = (buffer[1] << 8 | buffer[0]);
}

void removeIPD(unsigned char* src, unsigned char* dest)
{
	/* 
		Removes IPD from received data	 +IPD,ID,LEN:<data>
		generally, ID will be always 0, LEN always 6 (3x int16_t) = +IPD,0,6:
		It means we have to skip (or remove) 8 chars from source and the rest copy to destination
		Counting starts from 0, so char no. 9 will be the first copyable character
	*/
	
	int i = 0, offset = 8;
	uint8_t size = atoi(&src[7]);

	while (i < size)
	{
		dest[i++] = src[i+offset];
	}
}

int main(void)
{	
	unsigned char MUX[]		= "AT+CIPMUX=1";
	unsigned char MODE[]	= "AT+CWMODE=1";
	unsigned char CWJAP[]	= "AT+CWJAP=\"Shin\",\"haslo12345\"";
	unsigned char SERVER[]	= "AT+CIPSERVER=1";
	unsigned char SEND[]	= "AT+CIPSEND=0,17";

	struct sDataPacket sData;
	struct sControlPacket sControl;
	
	unsigned char receivedData[17];
	unsigned char controlPacket[sizeof(sControl)];
	LCD_Initalize();
	_delay_ms(1000);
	LCD_Clear();
	
	I2C_SetBitrate(100);
	LSM303D_Init_I2C();
	
	sData.__CR = 0x0D;
	sData.__LF = 0x0A;
	sData.__0 = 0;
	
	SerialInit();
	SerialTransmit(MUX);	
	SerialTransmit(MODE);
	SerialTransmit(CWJAP);
	SerialTransmit(SERVER);

	while(1)
	{
		SerialReceive(receivedData, sizeof(receivedData));
		removeIPD(receivedData, controlPacket);
		ReceiveStruct(&sControl, controlPacket, 6);
		
		readSensors(sData);
		SerialTransmit(SEND);
		SendStruct(&sData);
		
	} 
}
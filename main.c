#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
#include "i2c.h"
#include "lsm303d.h"

void sendStruct(struct sDataPacket *src);
void recieveStruct(struct sControlPacket *dst, uint8_t size);

struct sDataPacket
{
	uint16_t _accX;
	uint16_t _accY;
	uint16_t _accZ;
	uint16_t _temp;
	uint16_t _magX;
	uint16_t _magY;
	uint16_t _magZ;
	const unsigned char __CR	= 0x0D;	// stala wymagana do wyslania do ESP
	const unsigned char __LF	= 0x0A; // stala wymagana do wyslania do ESP
	const unsigned char __0		= 0;	// stala wymagana do wyslania do ESP
} sData;

struct sControlPacket
{
	int16_t _xAxis;
	int16_t _yAxis;
	int16_t _buttons;
} sControl;

uint16_t readLSM303D(uint8_t addr) 
{
	uint8_t bufor[2];
	TWI_read_buf(LSM303D_ADDR, addr, 8, bufor);
	return (((bufor[1]) << 8) | (bufor[0]));
}

int main(void)
{
	LCD_Initalize();
	I2C_SetBitrate(100);
	LSM303D_Init_I2C();

	while(1)
	{
		receiveStruct(sControl);	// odbieranie struktury od 3DS
		//sControl._yAxis	- wyjscie na tylne silniki
		//sControl._xAxis	- wyjscie an serwo
		//sControl._buttons	- info o przyciskach, idize na diody itd


		sData._accX = readLSM303D(OUT_X_H_A);
		sData._accY = readLSM303D(OUT_Y_H_A);
		sData._accZ = readLSM303D(OUT_Z_H_A);
		sData._temp = readLSM303D(TEMP_OUT_H);
		sData._magX = readLSM303D(OUT_X_H_M);
		sData._magY = readLSM303D(OUT_Y_H_M);
		sData._magZ = readLSM303D(OUT_Z_H_M);
		
		sendStruct(sData);			// wysylanie struktury do 3DSa
	} 
}


void sendStruct(struct sDataPacket *src)
{
	char *srcPointer = src;

	char size = sizeof(*src);

	char i;
	for (i = 0; i < size; i++)
	{
		SerialTransmitChar(*srcPointer++);
	}
}

void recieveStruct(struct sControlPacket *dst, uint8_t size)
{
	char *dstPointer = dst;

	char i;
	for (i = 0; i < size; i++)
	{
		*dstPointer = SerialReceiveChar();
		dstPointer++;
	}
}
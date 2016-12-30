#include <avr/io.h>  
#include "i2c.h"  
 
#define I2CDir DDRC  
#define I2COut PORTC  
#define I2CIn  PINDC 
//  
#define SDA 5  
#define SCL 6  

#define SET_SDA I2COut |= (1 << SDA)  
#define CLR_SDA I2COut &= ~(1 << SDA)  

#define SET_SCL I2COut |= (1 << SCL)  
#define CLR_SCL I2COut &= ~(1 << SCL)  

#define SDA_OUT I2CDir |= (1 << SDA)  
#define SDA_IN  I2CDir &= ~(1 << SDA)  

#define SCL_OUT I2CDir |= (1 << SCL)  
#define SCL_IN  I2CDir &= ~(1 << SDA)  

#define GET_SDA (I2CIn & (1 << SDA))  

#define ACK 1  
#define NACK 0  

void delay(void)  
{  
	asm("nop");  
	asm("nop");  
	asm("nop");  
	asm("nop");  
}  

// funkcja generujaca sygna³ start  
void I2C_Start(void)  
{  
	SDA_OUT; //  
	SCL_OUT; // ustawienie linii SDA i SCL w tryb wyjœciowy  
	SET_SDA; //  
	SET_SCL; // ustawienie na liniach SDA i SCL stanu wysokiego  
	delay(); // opóŸnienie  
	CLR_SDA;  
	delay();  
	CLR_SCL;  
}  

// funkcja generujaca sygna³ stop  
void I2C_Stop(void)  
{  
	CLR_SDA;  
	delay();  
	SET_SCL;  
	delay();  
	SET_SDA;  
	delay();  
}  
  
// funkcja wysy³aj¹ca bajt na szynê I2C  
void I2C_Write(unsigned char x)  
{  
	unsigned char count = 9;  
	do {  
		CLR_SCL;  
		if(x & 0x80)
		{  
			SET_SDA;  
		}
		else
		{  
			CLR_SDA;  
		}  
		x <<= 1;  
		delay();  
		SET_SCL;  
		delay();  
	} while(--count);  
	CLR_SCL;  
}  
  
// funkcja odczytujaca bajt z szyny I2C  
unsigned char I2C_Read(unsigned char ack)  
{  
	unsigned char count = 8, temp = 0;  
	SET_SDA;  
	SDA_IN;  
	do {  
		delay();  
		SET_SCL;  
		delay();  
		temp <<= 1;  
		if(GET_SDA)
		{  
			temp++;  
		}  
		CLR_SCL;  
	} while(--count);  
	if(ack)
	{  
		SET_SDA;  
	}	  
	delay();  
	SET_SCL;  
	delay();  
	CLR_SCL;  
	return (temp);  
}  
  
//odczyt z I2C spod urz¹dzenia o adresie 'SLA' z podadresu 'adr' iloœci  
//bajtów 'len' i zachowanie w 'buf'  
void I2C_readBuf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf)  
{  
	I2C_Start();  
	I2C_Write(sla);  
	I2C_Write(adr);  
	I2C_Start();  
	I2C_Write(sla + 1);  
	while(len--)
	{  
		*buf++ = I2C_Read(len ? ACK : NACK);  
	}  
	I2C_Stop();  
}  
  
//zapis na I2C do urz¹dzenia o adresie 'SLA' do podadresu 'adr' iloœci   
//bajtów 'len' spod podrêcznego 'buf'  
void I2C_writeBuf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf)  
{  
	I2C_Start();  
	I2C_Write(sla); //wysy³ka adresu slave-a  
	I2C_Write(adr); //wysy³ka adresu rejestru slave-a (SUB)  
	while(len--)
	{  
		I2C_Write(*buf++);  //wysy³ka danych  
	}  
	I2C_Stop(); //koniec transmisji  
}  
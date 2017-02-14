#include <avr/io.h>
#include <util/delay.h>

#include "i2c.h"
#include "uart.h"
#include "lsm303d.h"

#define L_IN_1		(1<<PC3)
#define L_IN_2		(1<<PC2)
#define PWM_L		(1<<PD7)	// OC2A

#define P_IN_1		(1<<PA2)
#define P_IN_2		(1<<PA3)
#define PWM_P		(1<<PB4)	// OC0B

#define PWM_SERWO	(1<<PD5)	// OC1A

void initMotors(void)
{
	/*
		Initalizes rear motors, pins connected to H-bridge
		2 pins for control and 1 for speed (PWM)
	*/
	
	DDRA |= (P_IN_1 | P_IN_2);
	DDRB |= (PWM_P);
	DDRC |= (L_IN_1 | L_IN_2);
	DDRD |= (PWM_L);
	
	TCCR0A |= (1 << COM0B1 | 1 << WGM01 | 1<< WGM00);
	TCCR0B |= (1 << CS00);
	
	TCCR2A |= (1 << COM2A1 | 1 << WGM21 | 1 << WGM20);
	TCCR2B |= (1 << CS20);
	
	OCR0B = 0;
	OCR2A = 0;
	
}

void runMotors(char dir, char power)
{
	/*
		Driving the motors
		"dir" means which direction (forward od backward)
		"power" means power, obviously, values betwen 0-255
	*/
	
	switch (dir){
		// FORWARD
		case 1:
			PORTC &= ~(L_IN_2);
			PORTC |= (L_IN_1);
			
			PORTA &= ~(P_IN_2);
			PORTA |= (P_IN_1);
			
			OCR0B = power;
			OCR2A = power;
			break;
		
		// BACKWARD
		case 2:
			PORTC &= ~(L_IN_1);
			PORTC |= (L_IN_2);
			
			PORTA &= ~(P_IN_1);
			PORTA |= (P_IN_2);
		
					
			OCR0B = power;
			OCR2A = power;
			break;
		
		// STOP
		case 0:
			PORTC &= ~(L_IN_1 | L_IN_2);
			PORTA &= ~(P_IN_1 | P_IN_2);
			
			OCR0B = 0;
			OCR2A = 0;
			break;
			
		// also STOP
		default:
			PORTC &= ~(L_IN_1 | L_IN_2);
			PORTA &= ~(P_IN_1 | P_IN_2);
			
			OCR0B = 0;
			OCR2A = 0;
			break;
	}
}

void initServo(void)
{
	/*
		Initalizes servomotor, only one pin needed to control (PWM)
	*/
	DDRD |= (PWM_SERWO);
	TCCR1A |= (1 <<COM1A1 | 1 << WGM11);
	TCCR1B |= (1 << WGM13 | 1<< CS11 | 1<< CS10); // prescaler /64 - timer period euqals to 3.74us
	ICR1 = 5759; // max value
}

void moveServo(int value){
	/*
		Moves the servo left or right
		As i remember, max-left position is 200, max-right position was 300
	*/
	OCR1A = value;
}

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

	initMotors();
	initServo();
	unsigned char MUX[]		= "AT+CIPMUX=1";
	unsigned char MODE[]	= "AT+CWMODE=1";
	unsigned char CWJAP[]	= "AT+CWJAP=\"Shin\",\"haslo12345\"";
	unsigned char SERVER[]	= "AT+CIPSERVER=1";
	unsigned char SEND[]	= "AT+CIPSEND=0,17";

	struct sDataPacket sData;
	struct sControlPacket sControl;
	
	unsigned char receivedData[17];
	unsigned char controlPacket[sizeof(sControl)];

	
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
		
		runMotors(1,250):
		
	} 
}
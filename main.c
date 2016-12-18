#define F_CPU 16000000UL
#include <avr/io.h> 
#include <util/delay.h>  

#include "usart.h"
#define FOSC F_CPU
#define BAUD 115200

int main(void)  
{  	
	SerialInit(	FOSC, 	// Szybkosc taktowania zegara		\
				BAUD, 	// Wymagany baud rate, wartosci 	/ wymagane do  ustawienia rejestru
				8, 		// 8 bitow danych
				1, 		// 1 bit stopu
				0);		// bez parzystosci
				
	char rx; 		// bufor do odbierania
	while(1)
	{
		
	}
	
	return 0; 
} 


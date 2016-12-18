void SerialInit(unsigned long int 	fosc,
				unsigned int 		baud,
				short int 			bits,
				short int 			stopBits,
				short int 			patity);
				
void SerialTransmitChar(unsigned char data);

void SerialTransmit(unsigned char *s);

unsigned char SerialReceive(void);
void SerialInit(unsigned long int 	fosc,
				unsigned long int 	baud,
				unsigned char		speed,
				short int 			bits,
				short int 			stopBits,
				short int 			patity);
				
void SerialTransmitChar(unsigned char data);

void SerialTransmit(unsigned char s[]);

unsigned char SerialReceiveChar(void);

unsigned char SerialReceive(unsigned char* dest, unsigned char size);

void SerialFlush(void);
void 			SerialInit(	short int bits, short int stopBits, short int patity );
void 			SerialTransmitChar( unsigned char data );
void 			SerialTransmit( unsigned char s[] );
unsigned char 	SerialReceiveChar( void );
unsigned char 	SerialReceive( unsigned char* dest, unsigned char size );
void 			SerialFlush( void );
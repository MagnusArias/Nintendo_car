struct sDataPacket
{
	int16_t _accX;
	int16_t _accY;
	int16_t _accZ;
	int16_t _temp;
	int16_t _magX;
	int16_t _magY;
	int16_t _magZ;
	unsigned char __CR;
	unsigned char __LF;
	unsigned char __0;
};

struct sControlPacket
{
	int16_t _xAxis;
	int16_t _yAxis;
	int16_t _buttons;
};

void 			SerialInit( void );
void 			SerialTransmitChar( unsigned char data );
void 			SerialTransmit( unsigned char s[] );
unsigned char 	SerialReceiveChar( void );
unsigned char 	SerialReceive( unsigned char* dest, unsigned char size );
void 			SerialFlush( void );

void			SendStruct( struct sDataPacket *src );
void			ReceiveStruct( struct sControlPacket *dst, uint8_t size );
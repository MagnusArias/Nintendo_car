struct sDataPacket
{
	int16_t _accX;			// accelerometer X-axis
	int16_t _accY;			// accelerometer Y-axis
	int16_t _accZ;			// accelerometer Z-axis
	int16_t _temp;			// temperature
	int16_t _magX;			// magnetometer X-axis
	int16_t _magY;			// magnetometer Y-axis
	int16_t _magZ;			// magnetometer Z-axis
	unsigned char __CR;		// carriage return					It's necessary to send data from
	unsigned char __LF;		// line feed						AVR to ESP8266 with UART
	unsigned char __0;		// last char of whole struct		without any data loss or errors 
};

struct sControlPacket
{
	int16_t _xAxis;			// position of X-axis on 3DS (signed value, from [-1...+1]
	int16_t _yAxis;			// position of Y-axis on 3DS (signed value, from [-1...+1]
	int16_t _buttons;		// register with held buttons on 3DS
};

void 			SerialInit( void );
void 			SerialTransmitChar( unsigned char data );
void 			SerialTransmit( unsigned char s[] );
unsigned char 	SerialReceiveChar( void );
unsigned char 	SerialReceive( unsigned char* dest, unsigned char size );
void 			SerialFlush( void );
void			SendStruct( struct sDataPacket *src );
void			ReceiveStruct( struct sControlPacket *dst, unsigned char* src, uint8_t size );
#define ACK 1
#define NACK 0

void	I2C_SetBitrate( uint16_t );
void	TWI_start( void );
void	TWI_stop( void );
void	TWI_write (uint8_t ) ;
uint8_t TWI_read( uint8_t );
void	TWI_write_buf( uint8_t, uint8_t, uint8_t, uint8_t* );
void	TWI_read_buf( uint8_t, uint8_t, uint8_t, uint8_t* );
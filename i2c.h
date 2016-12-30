#ifndef I2C_H_  
#define I2C_H_  
  
//#define ACK 1  
//#define NACK 0  
  
void 			delay(void);  
void 			I2C_Start(void);  
void 			I2C_Stop(void);  
void 			I2C_Write(unsigned char x);  
unsigned char 	I2C_read(unsigned char ack);  
void 			I2C_ReadBuf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf);  
void 			I2C_WriteBuf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf );  
  
#endif /* I2C_H_ */  
/*
 * lsm303d.h
 *
 * Created: 03.01.2017 17:49:33
 *  Author: Magnus
 */ 


#ifndef LSM303D_H_
#define LSM303D_H_

#define LSM303D_ADDR	0x3A
#define ADDR_WRITE		0x3A
#define ADDR_READ		0x3B

#define TEMP_OUT_L		0x05
#define TEMP_OUT_H		0x06

#define STATUS_M		0x07
#define OUT_X_L_M       0x08            
#define OUT_X_H_M       0x09
#define OUT_Y_L_M       0x0A
#define OUT_Y_H_M       0x0B
#define OUT_Z_L_M       0x0C
#define OUT_Z_H_M       0x0D

#define WHO_I_AM		0x0F
#define INT_CTRL_M		0x12
#define INT_SRC_M		0x13
#define INT_THS_L_M		0x14
#define INT_THS_H_M		0x15

#define OFFSET_X_L_M    0x16
#define OFFSET_X_H_M    0x17
#define OFFSET_Y_L_M    0x18
#define OFFSET_Y_H_M    0x19
#define OFFSET_Z_L_M    0x1A
#define OFFSET_Z_H_M    0x1B

#define REFERENCE_X		0x1C
#define REFERENCE_Y		0x1D
#define REFERENCE_Z		0x1E

#define CTRL0           0x1F
#define CTRL1           0x20
#define CTRL2           0x21
#define CTRL3           0x22
#define CTRL4           0x23
#define CTRL5           0x24
#define CTRL6           0x25
#define CTRL7           0x26

#define STATUS_A        0x27
#define OUT_X_L_A       0x28
#define OUT_X_H_A       0x29
#define OUT_Y_L_A       0x2A
#define OUT_Y_H_A       0x2B
#define OUT_Z_L_A       0x2C
#define OUT_Z_H_A       0x2D

#define FIFO_CTRL       0x2E            // Rejestr kontrolny bufora FIFO, typu read/write.
#define FIFO_STAT       0x2F            // Status rejestru bufora FIFO, typu read.

#define WriteCTRL1      0x27            // Makro wybudzaj¹ce akcelerometr, oraz ustawiaj¹ce odswiezanie na 6.25Hz
#define WriteCTRL5      0x84			// TEMP_EN = 1; M_ODR- 6.25 Hz
#define WriteCTRL7      0xA0            // High-pass filter mode selection: normal mode & Magnetic sensor mode selection: Continuous-conversion mode

void LSM303D_Init_I2C(void)
{
	TWI_start();
	TWI_write(ADDR_WRITE);
	TWI_write(CTRL1);
	TWI_write(WriteCTRL1);
	TWI_stop();
	
	TWI_start();
	TWI_write(ADDR_WRITE);
	TWI_write(CTRL5);
	TWI_write(WriteCTRL5);
	TWI_stop();
		
	TWI_start();
	TWI_write(ADDR_WRITE);
	TWI_write(CTRL7);
	TWI_write(WriteCTRL7);
	TWI_stop();
}

#endif /* LSM303D_H_ */
#ifndef I2C_PROTOCOL_H_
#define I2C_PROTOCOL_H_


typedef void (*I2C_INIT)();

typedef void (*I2C_WRITE_BYTE)(unsigned char data, unsigned char addr);

typedef void (*I2C_READ_BYTES)(unsigned char addr, int length, unsigned char *buf);

typedef struct _I2cModeOps {
	I2C_INIT I2cInit;
	I2C_WRITE_BYTE I2cWriteByte;
	I2C_READ_BYTES I2cReadBytes;
} I2cModeOps;

typedef struct _I2cAdpter
{
	int speed;
	int slaveAddr;
	int isInitDone;
	I2cModeOps* i2cModeOps; 
} I2cAdpter;

#endif
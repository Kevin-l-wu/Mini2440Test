#include "GlobalDefine.h"
#include "Error.h"
#include "Common.h"
#include "I2c.h"

#include "ModManager.h"
#include "../Protocol/inc/I2cProtocol.h"

#define INTPND (*(volatile unsigned long*)0x4a000010)
#define SRCPND (*(volatile unsigned long*)0x4a000000)
#define INTMSK (*(volatile unsigned long*)0x4a000008)
#define GPECON (*(volatile unsigned long*)0x56000040)
#define GPEUP  (*(volatile unsigned long*)0x56000048)

#define IICCON    (*(volatile unsigned char*)0x54000000)
#define IICSTAT   (*(volatile unsigned char*)0x54000004)
#define IICDS     (*(volatile unsigned char*)0x5400000C)

#define SLAVE_WRITE_ADDR 0xa0
#define SLAVE_READ_ADDR 0xa1

static void I2cDelay(int i)
{
   int j = 0;
   while (i--)	
   {
       for (j = 0; j < 100; j++)
       {	
           ;
       }  
   }	
}

static void I2cInit()
{
    //1 Interrupt Initialize
    INTPND |= (1 << 27);
    SRCPND |= (1 << 27);  
    INTMSK &= ~(1 << 27);
     
    IICCON |= (1 << 5); 
    
    //2 Set I2C-Bus transmit clock
    IICCON &= ~(1 << 6);
    IICCON &= ~(0xf << 0);
    IICCON |= (0x5 << 0);
    
    //3. Set IIC-bus data output enable
    IICSTAT |= (1 << 4);
    
    //4. Set GPIO pin function
    GPECON |= (0x2 << 28)|(0x2 << 30);
    GPEUP |= (0x3 << 14);
    
    //5. Set IIC-bus acknowledge enable
    IICCON |= (1 << 7);
}

static void I2cWriteByte(unsigned char data, unsigned char addr)
{    
    //1. Set to Master-Transmit mode
    IICSTAT |= (3 << 6);
    
    //2. Write slave device address
    IICDS = SLAVE_WRITE_ADDR;
    IICCON &= ~(1 << 4);
    
    //3. Write 0xF0 to IICSTAT.(Generate START signal) 
    IICSTAT = 0xF0;
    
    // Wait Ack
    while ((IICCON & (1 << 4)) == 0 )
        I2cDelay(100);
    
    //4. Write address in chip to IICDS
    IICDS = addr;
    IICCON &= ~(1 << 4);
    
    // Wait Ack
    while ((IICCON & (1 << 4)) == 0 )
        I2cDelay(100);
    
    //5. Write data to IICDS
    IICDS = data;
    IICCON &= ~(1 << 4);   
    
    // Wait Ack
    while ((IICCON & (1 << 4)) == 0 )
        I2cDelay(100);
    
    //6. Write 0xD0 to IICSTAT(Generate STOP signal)
    IICSTAT = 0xD0;
    
    //7. Clear Interrupt
    IICCON &= ~(1 << 4);	
    
    I2cDelay(100);
}

static void I2cReadBytes(unsigned char addr, int length, unsigned char *buf)
{
    int j = 0;
    unsigned char unusedata;
    
    //1. Set to Master-Transmit mode
    IICSTAT |= (3 << 6);
    
    //2. Write slave device write address
    IICDS = SLAVE_WRITE_ADDR;
    IICCON &= ~(1 << 4);
    
    //3. Write 0xF0 to IICSTAT
    IICSTAT = 0xF0;
    
    //Wait ACK
    while ((IICCON & (1 << 4)) == 0 )
        I2cDelay(100);
    
    //4. Write address in the eeprom
    IICDS = addr;
    IICCON &= ~(1 << 4);
    
    // Wait ACK
    while ((IICCON & (1 << 4)) == 0 )
        I2cDelay(100);
             
    //5. Set to Master-Receive mode
    IICSTAT &= ~(3 << 6);
    IICSTAT |= (2 << 6);
    
    //6. Write slave device read address
    IICDS = SLAVE_READ_ADDR;
    IICCON &= ~(1 << 4);
    
    //7. Write 0xB0 to IICSTAT for starting to receive
    IICSTAT = 0xb0;
    while ((IICCON & (1 << 4)) == 0 )
        I2cDelay(100);
	
    //8. Write address in chip
	IICDS = addr;
	IICCON &= ~(1 << 4);
	
	while((IICCON & (1 << 4)) == 0)
	{
		I2cDelay(100);
	}
  
    for(j = 0; j < length; j++)
    {
        if(j == (length - 1))
        {
           IICCON &= ~(1 << 7); 		
        }
   
        buf[j] = IICDS;
		
		// Clear Interrupt
        IICCON &= ~(1 << 4);
    
		// Wait for Interrupt
        while ((IICCON & (1 << 4)) == 0 )
            I2cDelay(100);
    }
	
    //9. Write 0x90 to IICSTAT(Generate STOP signal)
    IICSTAT = 0x90;
    
    //10. Clear Interrupt
    IICCON &= ~(1 << 4);
}

I2cModeOps i2cModeOps = {
	.I2cInit = I2cInit,
	.I2cWriteByte = I2cWriteByte,
	.I2cReadBytes = I2cReadBytes,
};

MODULE_INSTALL(I2c, MOD_I2C, 0, &i2cModeOps);
#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"
#include "Common.h"
#include "GetoptTemp.h"

#include "ModManager.h"
#include "../Protocol/inc/I2cProtocol.h"
#include "CommandManager.h"


I2cModeOps* gI2cModeOps = NULL;


void MiscTest()
{
	int i = 0;
    unsigned char sbuf[256] = {0};
    unsigned char dbuf[256] = {0};	
	
	for(i = 0; i < 256; i++)
	{
		sbuf[i] = i;
		dbuf[i] = 0;
	}
	
	printf_string("dbuf befor I2C read:\r\n");
	
	for(i = 0; i < 256;i++)
	{
		if(i % 8 == 0)
			printf_string("\r\n");

		printf_string("%d\t",dbuf[i]);	
	}	
	
	for(i = 0; i < 256; i++)
		gI2cModeOps->I2cWriteByte(sbuf[i], i);
		
	printf_string("i2c reading, plese wait!\n");
	
	gI2cModeOps->I2cReadBytes(0, 256, dbuf);
	
	printf_string("dbuf after I2C read:\n");
	
	for(i = 0; i < 256; i++)
	{
	   if(i % 8 == 0)
		   printf_string("\r\n");
		   
	   printf_string("%d\t", dbuf[i]);	
	}
}


MINI2440_STATUS TestI2c(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	char option = 0;
	unsigned char address = 0x00;
	unsigned char data;
	int length = 0;
	int i = 0;
	
	unsigned char buf[256] = {0};
	
	if(!gI2cModeOps)
	{
		gLocateProtocol(MOD_I2C, (void*)&gI2cModeOps);
	}
	
	if(gI2cModeOps != NULL)
	{
		gI2cModeOps->I2cInit();
		
		GetoptInit();
		
		while ((option = Getopt(argc, argv, "rwt")) != 0)
		{
			switch(option)
			{
				case 'r':
					address = (unsigned char)hex_string_to_int(argv[optInd]);					
					printf_string("address = 0x%x\n", address);
					
					length = hex_string_to_int(argv[optInd + 1]);
					printf_string("length = 0x%x\n", length);
					
					gI2cModeOps->I2cReadBytes(address, length, buf);
					
					printf_string("Read Content(0x%x):\n", address);
					
					for(i = 0; i < length; i++)
					{
						printf_string("0x%x\n", buf[i]);
					}
					
					break;
				
				case 'w':
					address = (unsigned char)hex_string_to_int(argv[optInd]);					
					printf_string("address = 0x%x\n", address);
					
					data = (unsigned char)hex_string_to_int(argv[optInd + 1]);
					printf_string("data = 0x%x\n", data);
					
					gI2cModeOps->I2cWriteByte(data, address );
					
					break;
				
				case 't':
					MiscTest();
					break;
				
				default:
					printf_string("Invalid Parameter\n");
					break;
			}
		
		}
	}
}

#define I2C_COMMAND_HELP "\
I2c test\n\
	-r : Read a byte. i2c -r address(hex) length(hex)\n\
	-w : Write a byte. i2c -w address(hex) value(hex)\n\
	-t : Misc test. i2c -t\
	"

COMMAND_INSTALL(i2c, I2C_COMMAND_HELP, TestI2c);

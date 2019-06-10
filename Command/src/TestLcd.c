#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "Template.h"

#include "ModManager.h"
#include "../Protocol/inc/LcdProtocol.h"
#include "CommandManager.h"

LcdModOps* gLcdOps = NULL;
//extern unsigned char gImageTest[153608];

MINI2440_STATUS TestLcd(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int line_index = 0;
	int column_index = 0;
	int color_index = 0;
	TEMPLATE8X16* ascii_template;
	
	gLocateProtocol(MOD_LCD, (void*)&gLcdOps);
	
	if(gLcdOps != NULL)
	{
		gLcdOps->LcdModInit();
		
		print_string("Fill all screen test\n");
		
		
		gLcdOps->LcdDrawBox(0, 50, 100, 100, 0xf800);
		
		
		delay(1024 * 1024 * 2);
		
		
		gLcdOps->LcdClearScreen(0x0000);
		
		
		gLcdOps->LcdDrawRect(100, 100, 150, 150, 0xf800);
		
		delay(1024 * 1024 * 2);
		
		gLcdOps->LcdClearScreen(0x0000);
		
		delay(1024 * 1024 * 2);
//		gLcdOps->LcdDrawImage(0, 0, 240, 320, (unsigned short*)gImageTest);
		
		delay(1024 * 1024 * 2);
		
		gLcdOps->LcdClearScreen(0x0000);
		
		delay(1024 * 1024 * 2);
		
		/*
		
		for(color_index = 0; color_index < 15; color_index++)
		{
			printf_string("Color: %d\n\n", 0x03 << color_index);
			gLcdOps->LcdClearScreen(0x03 << color_index); 
			
			delay(500000);
		}
		
		gLcdOps->LcdClearScreen(0x0000);

		print_string("\nlcd line test\n");
		
		gLcdOps->LcdLineTest(0, 80, 0x0f);
		gLcdOps->LcdLineTest(80, 160, 0x0f << 4);
		gLcdOps->LcdLineTest(168, 169, 0x0f << 8);
		gLcdOps->LcdLineTest(240, 320, 0x0f << 12);
		
		gLcdOps->LcdClearScreen(0x0000);
		
		print_string("\nlcd column test\n");
		
		gLcdOps->LcdColumnTest(0, 60, 0x0f);
		gLcdOps->LcdColumnTest(60, 120, 0x0f << 4);
		gLcdOps->LcdColumnTest(125, 126, 0x0f << 8);
		gLcdOps->LcdColumnTest(180, 240, 0x0f << 12);
		
		gLcdOps->LcdClearScreen(0x0000);
		
		print_string("\nDisplay template\n");
		gLcdOps->LcdDisplayChar(50 + 16 * 0, 120 + 0, Global_ascii_template[0].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 1, 120 + 0, Global_ascii_template[1].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 2, 120 + 0, Global_ascii_template[2].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 3, 120 + 0, Global_ascii_template[3].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 4, 120 + 0, Global_ascii_template[4].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 5, 120 + 0, Global_ascii_template[5].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 6, 120 + 0, Global_ascii_template[6].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 7, 120 + 0, Global_ascii_template[7].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 8, 120 + 0, Global_ascii_template[8].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 9, 120 + 0, Global_ascii_template[9].ascii_template);
		
		gLcdOps->LcdDisplayChar(50 + 16 * 0, 120 + 32, Global_ascii_template[10].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 1, 120 + 32, Global_ascii_template[11].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 2, 120 + 32, Global_ascii_template[12].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 3, 120 + 32, Global_ascii_template[13].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 4, 120 + 32, Global_ascii_template[14].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 5, 120 + 32, Global_ascii_template[15].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 6, 120 + 32, Global_ascii_template[16].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 7, 120 + 32, Global_ascii_template[17].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 8, 120 + 32, Global_ascii_template[18].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 9, 120 + 32, Global_ascii_template[19].ascii_template);
		
		gLcdOps->LcdDisplayChar(50 + 16 * 0, 120 + 64, Global_ascii_template[20].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 1, 120 + 64, Global_ascii_template[21].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 2, 120 + 64, Global_ascii_template[22].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 3, 120 + 64, Global_ascii_template[23].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 4, 120 + 64, Global_ascii_template[24].ascii_template);
		gLcdOps->LcdDisplayChar(50 + 16 * 5, 120 + 64, Global_ascii_template[25].ascii_template);
		
		delay(2000000);
		
		gLcdOps->LcdClearScreen(0x0000);
		
		get_template('A', 0xf800, &ascii_template);
		
		gLcdOps->LcdDisplayChar(50 + 16 * 0, 120 + 64, *ascii_template);
		
		get_template('B', 0x001f, &ascii_template);
		
		gLcdOps->LcdDisplayChar(50 + 16 * 1, 120 + 64, *ascii_template);
		
		get_template('C', 0x07e0, &ascii_template);
		
		gLcdOps->LcdDisplayChar(50 + 16 * 2, 120 + 64, *ascii_template);
		
		printf_string("ascii_template = 0x%x\n", ascii_template);
		printf_string("*ascii_template = 0x%x\n", *ascii_template);
		
		delay(2000000);
		
		gLcdOps->LcdClearScreen(0x0000);
		
		gLcdOps->LcdDisplayStringWithColor(50 + 16 * 0, 20 + 32 * 0, "DIAGVERSION", 0x07e0);
		gLcdOps->LcdDisplayStringWithColor(50 + 16 * 0, 20 + 32 * 1, "SN", 0xf800);
		gLcdOps->LcdDisplayStringWithColor(50 + 16 * 0, 20 + 32 * 2, "LEDTEST", 0x001f);
		gLcdOps->LcdDisplayStringWithColor(50 + 16 * 0, 20 + 32 * 3, "LCDTEST", 0x07e0);
		gLcdOps->LcdDisplayStringWithColor(50 + 16 * 0, 20 + 32 * 4, "NANDTEST", 0xf800);

		*/
		delay(2000000);	
	}
	else
	{
		status = MINI2440_LOAD_PROTOCOL_FAIL;
	}
	
	return status;
}

COMMAND_INSTALL(lcd, "\tLcd test", TestLcd);

#include "GlobalDefine.h"
#include "Template.h"
#include "Common.h"
#include "Stdio.h"
#include "Error.h"
#include "Uart.h"
#include "Lcd.h"

//#define LCD_DEBUG 1

#include "ModManager.h"
#include "../Protocol/inc/LcdProtocol.h"

#define rGPCCON		(*(volatile unsigned *)0x56000020) //Port C control
#define rGPCDAT		(*(volatile unsigned *)0x56000024) //Port C data
#define rGPCUP		(*(volatile unsigned *)0x56000028) //Pull-up control C

#define rGPDCON		(*(volatile unsigned *)0x56000030) //Port D control
#define rGPDDAT		(*(volatile unsigned *)0x56000034) //Port D data
#define rGPDUP		(*(volatile unsigned *)0x56000038) //Pull-up control D

#define rGPGCON		(*(volatile unsigned *)0x56000060) //Port G control
#define rGPGDAT		(*(volatile unsigned *)0x56000064) //Port G data
#define rGPGUP		(*(volatile unsigned *)0x56000068) //Pull-up control G

// LCD CONTROLLER
#define rLCDCON1	(*(volatile unsigned *)0x4d000000) //LCD control 1
#define rLCDCON2	(*(volatile unsigned *)0x4d000004) //LCD control 2
#define rLCDCON3	(*(volatile unsigned *)0x4d000008) //LCD control 3
#define rLCDCON4	(*(volatile unsigned *)0x4d00000c) //LCD control 4
#define rLCDCON5	(*(volatile unsigned *)0x4d000010) //LCD control 5

#define rLCDSADDR1	(*(volatile unsigned *)0x4d000014) //STN/TFT Frame buffer start address 1
#define rLCDSADDR2	(*(volatile unsigned *)0x4d000018) //STN/TFT Frame buffer start address 2
#define rLCDSADDR3	(*(volatile unsigned *)0x4d00001c) //STN/TFT Virtual screen address set
#define rREDLUT		(*(volatile unsigned *)0x4d000020) //STN Red lookup table
#define rGREENLUT	(*(volatile unsigned *)0x4d000024) //STN Green lookup table 
#define rBLUELUT	(*(volatile unsigned *)0x4d000028) //STN Blue lookup table
#define rDITHMODE	(*(volatile unsigned *)0x4d00004c) //STN Dithering mode
#define rTPAL		(*(volatile unsigned *)0x4d000050) //TFT Temporary palette
#define rLCDINTPND	(*(volatile unsigned *)0x4d000054) //LCD Interrupt pending
#define rLCDSRCPND	(*(volatile unsigned *)0x4d000058) //LCD Interrupt source
#define rLCDINTMSK	(*(volatile unsigned *)0x4d00005c) //LCD Interrupt mask
#define rTCONSEL	(*(volatile unsigned *)0x4d000060) //LPC3600 Control --- edited by junon


//------------Lcd controller 1 set-----------
#define CLKVAL 4

//------------Lcd controller 2 set-----------
#define VBPD 1
#define LINEVAL 320
#define VFPD 5
#define VSPW 1

//------------Lcd controller 3 set------------
#define HBPD 36
#define HOZVAL 250
#define HFPD 19

//------------Lcd controller 4 set------------

#define HSPW 5

#define SCR_XSIZE HOZVAL
#define SCR_YSIZE LINEVAL

#define LCD_ADDR ((U32)lcd_buffer)
#define LCD_CON5 ((1 << 11) | (1 << 9) | (1 << 8) | (1 << 3) | (1 << 0))

unsigned short lcd_buffer[SCR_YSIZE][SCR_XSIZE];

/**************************************************************
320×240 TFT LCD数据和控制端口初始化
**************************************************************/
static void LcdPortInit()
{
    rGPCUP = 0xffffffff; // Disable Pull-up register
    rGPCCON = 0xaaaa02a8; //Initialize VD[7:0],VM,VFRAME,VLINE,VCLK

    rGPDUP = 0xffffffff; // Disable Pull-up register
    rGPDCON = 0xaaaaaaaa; //Initialize VD[15:8]
}

/**************************************************************
320×240 TFT LCD功能模块初始化
**************************************************************/
static void LcdInit()
{
	rLCDCON1 = (CLKVAL << 8) | (3 << 5) | (12 << 1);
	rLCDCON2 = (VBPD << 24) | ((LINEVAL - 1) << 14) | (VFPD << 6) | (VSPW << 0);
	rLCDCON3 = (HBPD << 19) | ((HOZVAL - 1) << 8) | (HFPD << 0);
	rLCDCON4 = (13 << 8) | (HSPW << 0);

    rLCDCON5 = LCD_CON5;

	//Frame buffer start address A[30:22] A[21:1]
    rLCDSADDR1 = ((LCD_ADDR >> 22) << 21) | (((LCD_ADDR >> 1) & 0x1fffff) << 0);
    
	//Frame buffer end address A[21:1]
	rLCDSADDR2 = ((LCD_ADDR + HOZVAL * LINEVAL * 2) & 0x1fffff) >> 1;
    
	rLCDSADDR3 = HOZVAL;       

    rLCDINTMSK |= 3;
	rTCONSEL &= (~7);

	rTPAL = 0;
	rTCONSEL &= ~((1 << 4) | 1);
}

/**************************************************************
LCD视频和控制信号输出或者停止，1开启视频输出
**************************************************************/
static void LcdOutputEnable(int onoff)
{
    if(onoff == 1)
	{
		rLCDCON1 |= 0x01; // ENVID=ON
    }
	else
	{
		rLCDCON1 &= (~0x01); // ENVID Off
	}
}

/**************************************************************
320×240 8Bpp TFT LCD 电源控制引脚使能
**************************************************************/
static void LcdPowerEnable(int invpwren, int pwren)
{
    //GPG4 is set as LCD_PWREN
    rGPGUP = rGPGUP | (1 << 4); // Pull-up disable
    rGPGCON = rGPGCON | (3 << 8); //GPG4=LCD_PWREN
    
    //Enable LCD POWER ENABLE Function
    rLCDCON5 = rLCDCON5 & (~(1 << 3)) | (pwren << 3);   // PWREN
    rLCDCON5 = rLCDCON5 & (~(1 << 5)) | (invpwren << 5);   // INVPWREN
}


static int LcdDrawPixel(int x, int y, U16 color)
{
	int ret = -1;
	
	if(y < SCR_YSIZE && x < SCR_XSIZE)
	{
		lcd_buffer[y][x] = color ;
		ret  =0;
	}
	
	return ret;
}

static int LcdGetPixel(int x, int y, U16* color)
{
	int ret = -1;
	
	if((color != NULL) && (y < SCR_YSIZE) && (x < SCR_XSIZE))
	{
		*color = lcd_buffer[y][x];
		ret = 0;
	}
	
	return ret;
}

static int LcdDrawLine(int line_number, U16 color)
{
	int index = 0, ret = -1;
	
	if(line_number < SCR_YSIZE)
	{
		for(index = 0; index < SCR_XSIZE; index++)
		{
			lcd_buffer[line_number][index] = color;
		}
		
		ret = 0;
	}
	
	return ret;
}

static int LcdDrawColumn(int column_number, U16 color)
{
	int index = 0, ret = -1;
	
	if(column_number < SCR_XSIZE)
	{
		for(index = 0; index < SCR_YSIZE; index++)
		{
			lcd_buffer[index][column_number] = color;
		}
		
		ret = 0;
	}
	
	return ret;
}

static int LcdDrawBox(int x, int y, int width, int height, U16 color)
{
	int indexX = 0, indexY = 0;
	int ret = -1;
	
	if((x <= width) && (width <= SCR_XSIZE) && (y <= height) && (height <= SCR_YSIZE))
	{
		printf("LcdDrawBox()\n");
		
		for(indexX = x; indexX < width; indexX++)
		{
			for(indexY = y; indexY < height; indexY++)
			{
				lcd_buffer[indexY][indexX] = color;
			}
		}
		
		ret = 0;
	}
	
	return ret;
}

static int LcdDrawImage(int x, int y, int width, int height, U16* image)
{
	int indexX = 0, indexY = 0;
	int ret = -1;
	
	if((x <= width) && (width <= SCR_XSIZE) && (y <= height) && (height <= SCR_YSIZE))
	{
		printf("LcdDrawBox()\n");
		
		for(indexX = x; indexX < width; indexX++)
		{
			for(indexY = y; indexY < height; indexY++)
			{
				lcd_buffer[indexY][indexX] = image[(indexY - y) * (width - x) + (indexX - x)];
			}
		}
		
		ret = 0;
	}
	
	return ret;
}


static int LcdDrawRect(int left, int bottom, int right, int top, U16 color)
{
	int indexX = 0, indexY = 0;
	int ret = -1;
	
	if((left <= right) && (right <= SCR_XSIZE) && (bottom <= top) && (top <= SCR_YSIZE))
	{
		printf("LcdDrawRect()\n");
		
		indexX = left;
		
		for(indexY = bottom; indexY < top; indexY++)
		{
			lcd_buffer[indexX][indexY] = color;
		}
		
		indexX = right;
		
		for(indexY = bottom; indexY < top; indexY++)
		{
			lcd_buffer[indexX][indexY] = color;
		}
		
		indexY = bottom;
		
		for(indexX = left; indexX < right; indexX++)
		{
			lcd_buffer[indexX][indexY] = color;
		}
		
		indexY = top;
		
		for(indexX = left; indexX < right; indexX++)
		{
			lcd_buffer[indexX][indexY] = color;
		}
		
		ret = 0;
	}
	
	return ret;
}

static void LcdClearScreen(U16 color)
{
	unsigned int x, y;
  
    for(y = 0; y < (SCR_YSIZE); y++)
    {
		for(x = 0; x < (SCR_XSIZE); x++)
		{
			lcd_buffer[y][x] = color ;
		}
    }
}

static void LcdDumpRegister()
{
	printf("Dump register:\n");
	printf("rLCDCON1 = %x\n", rLCDCON1);
	printf("rLCDCON2 = %x\n", rLCDCON2);
	printf("rLCDCON3 = %x\n", rLCDCON3);
	printf("rLCDCON4 = %x\n", rLCDCON4);
	printf("rLCDCON5 = %x\n", rLCDCON5);
	
	printf("rLCDSADDR1 = %x\n", rLCDSADDR1);
	printf("rLCDSADDR2 = %x\n", rLCDSADDR2);
	printf("rLCDSADDR3 = %x\n", rLCDSADDR3);
	
	printf("rLCDINTMSK = %x\n", rLCDINTMSK);
	
	printf("rTCONSEL = %x\n", rTCONSEL);
	printf("rTPAL = %x\n", rTPAL);
}

static void LcdLineTest(int start_line, int end_line, U16 color)
{
	int line_index = 0;
	int column_index = 0;
	int color_index = 0;
	
	printf("\nlcd_line_test, start_line = %d, end_line = %d\n", start_line, end_line);
	
	printf("Color: %d\n\n", color);
	for(line_index = start_line; line_index < end_line; line_index++)
	{
		printf("Fill line: %d\n\n", line_index);
		LcdDrawLine(line_index, color);
		
		delay(1000);
	}
	delay(1500000);
}

static void LcdColumnTest(int start_column, int end_column, U16 color)
{
	int line_index = 0;
	int column_index = 0;
	int color_index = 0;
	
	printf("\nlcd_column_test, start_column = %d, end_column = %d\n", start_column, end_column);

	printf("Color: %d\n\n", color);
	for(column_index = start_column; column_index < end_column; column_index++)
	{
		printf("Fill column: %d\n\n", column_index);
		LcdDrawColumn(column_index, color);
		
		delay(1000);
	}
	delay(1500000);
}

static void LcdDisplayChar(int column_number, int line_number, unsigned short* template)
{
	int x = 0, y = 0;
//	printf("LcdDisplayChar()\n");
	
	for(y = line_number; y < (line_number + 16); y++)
	{
		for(x = column_number; x < (column_number + 8); x++)
		{
			#ifdef LCD_DEBUG
			printf("x = %d, y = %d\n\n", x, y);
			#endif
			if(template[(x - column_number) + (y - line_number) * 8] != 0x00)
				lcd_buffer[y][x] = template[(x - column_number) + (y - line_number) * 8];
		}
	}
}

static void LcdDisplayCharWithColor(int column_number, int line_number, char ascii_char, unsigned short color)
{
	TEMPLATE8X16* ascii_template = NULL;
	
	get_template(ascii_char, color, &ascii_template);
	
	LcdDisplayChar(column_number, line_number, *ascii_template);
}

static void LcdDisplayStringWithColor(int column_number, int line_number, char* ascii_string, unsigned short color)
{
	int char_no = 0;
	
	if(ascii_string != NULL)
	{
		while(*ascii_string != '\0')
		{
			LcdDisplayCharWithColor(column_number + 16 * char_no, line_number, *ascii_string, color);
			char_no++;
			ascii_string++;
		}
	}
}

static void LcdModInit()
{
	LcdPortInit();
	
	LcdInit();
	
	LcdPowerEnable(0, 1);
	LcdOutputEnable(1);
}

static void LcdClose()
{
	
}

LcdModOps lcdModOps = {
	.LcdModInit = LcdModInit,
	.LcdDrawPixel = LcdDrawPixel,
	.LcdGetPixel = LcdGetPixel,
	.LcdDrawLine = LcdDrawLine,
	.LcdDrawColumn = LcdDrawColumn,
	.LcdDrawBox = LcdDrawBox,
	.LcdDrawRect = LcdDrawRect,
	.LcdClearScreen = LcdClearScreen,
	.LcdDrawImage = LcdDrawImage,
	.LcdDisplayChar = LcdDisplayChar,
	.LcdDisplayCharWithColor = LcdDisplayCharWithColor,
	.LcdDisplayStringWithColor = LcdDisplayStringWithColor,
	.LcdLineTest = LcdLineTest,
	.LcdColumnTest = LcdColumnTest,
	.LcdClose = LcdClose,
};

MODULE_INSTALL(Lcd, MOD_LCD, 0, &lcdModOps);



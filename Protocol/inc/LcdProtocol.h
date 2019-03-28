#ifndef LCDPROTOCOL_H_
#define	LCDPROTOCOL_H_

#include "GlobalDefine.h"

typedef void (*LCD_INIT)();

typedef int (*LCD_DRAW_PIXEL)(int x, int y, U16 color);

typedef int (*LCD_GET_PIXEL)(int x, int y, U16* color);

typedef int (*LCD_DRAW_LINE)(int line_number, U16 color);

typedef int (*LCD_DRAW_COLUMN)(int column_number, U16 color);

typedef int (*LCD_DRAW_BOX)(int left, int bottom, int right, int top, U16 color);

typedef int (*LCD_DRAW_RECT)(int left, int bottom, int right, int top, U16 color);

typedef void (*LCD_CLEAR_SCREEN)(U16 color);

typedef void (*LCD_DISPLAY_CHAR)(int column_number, int line_number, unsigned short* template);

typedef void (*LCD_DISPLAY_CHAR_WITH_COLOR)(int column_number, int line_number, char ascii_char, unsigned short color);

typedef void (*LCD_DISPLAY_STRINT_WITH_COLOR)(int column_number, int line_number, char* ascii_string, unsigned short color);

typedef void (*LCD_LINE_TEST)(int start_line, int end_line, U16 content);

typedef void (*LCD_COLUMN_TEST)(int start_column, int end_column, U16 content);

typedef void (*LCD_CLOSE)();


typedef struct _LcdModOps
{
	LCD_INIT LcdModInit;
	LCD_DRAW_PIXEL LcdDrawPixel;
	LCD_GET_PIXEL LcdGetPixel;
	LCD_DRAW_LINE LcdDrawLine;
	LCD_DRAW_COLUMN LcdDrawColumn;
	LCD_DRAW_BOX LcdDrawBox;
	LCD_DRAW_RECT LcdDrawRect;
	LCD_CLEAR_SCREEN LcdClearScreen;
	LCD_DISPLAY_CHAR LcdDisplayChar;
	LCD_DISPLAY_CHAR_WITH_COLOR LcdDisplayCharWithColor;
	LCD_DISPLAY_STRINT_WITH_COLOR LcdDisplayStringWithColor;
	LCD_LINE_TEST LcdLineTest;
	LCD_COLUMN_TEST LcdColumnTest;
	LCD_CLOSE LcdClose;
} LcdModOps;


#endif
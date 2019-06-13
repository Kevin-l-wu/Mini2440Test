#include <stdio.h>
#include "Xui.h"
#include "StdLib.h"

#include "ModManager.h"
#include "../Protocol/inc/LcdProtocol.h"


static int isRunning = 0;
XuiWindow* root = NULL;
XuiWindow* statusBar = NULL;

extern LcdModOps* gLcdOps;

int XuiOpen(int argc, char** argv)
{
	if(!isRunning)
	{
		statusBar = malloc(sizeof(XuiWindow));
		printf("statusBar = %x\n", statusBar);
		if(statusBar != NULL)
		{
			statusBar->startPoint.x = 0;
			statusBar->startPoint.y = 0;
			statusBar->width = 240;
			statusBar->height = 40;
			statusBar->widget = NULL;
			statusBar->type = XUI_WIN_CANVAS;
			statusBar->key = 0;
		}
		
		root = malloc(sizeof(XuiWindow));
		printf("root = %x\n", root);
		if(root != NULL)
		{
			root->startPoint.x = 0;
			root->startPoint.y = 40;
			root->width = 240;
			root->height = 280;
			root->widget = NULL;
			root->type = XUI_WIN_CANVAS;
			root->key = 0;
		}
		
		if(gLcdOps  == NULL)
			gLocateProtocol(MOD_LCD, (void*)&gLcdOps);
	
		if(gLcdOps != NULL)
		{
			gLcdOps->LcdModInit();
			isRunning = 1;
		}
	}
}
int XuiIsRunning()
{
	return isRunning;
}
void XuiClose()
{
	if(isRunning)
	{
		isRunning = 0;
	}
}

int XuiSuspend()
{
	
}

int XuiResume()
{
	
}

XuiWindow* XuiRootCanvas()
{
	return root;
}
XuiWindow* XuiStatusbarCanvas()
{
	return statusBar;
}

XuiFont* XuiCreateFont(char* fontFile, int index, XuiFontSet fontSet)
{
	
}

void XuiDestroyFont(XuiFont* font)
{
	
}

int XuiCanvasDrawText(XuiWindow *window, unsigned int x, unsigned int y, unsigned int height,
	XuiFont *font, XuiTextStyle textstyle, XuiColor fg, char *text)
{
	if(window != NULL && text != NULL)
	{
		if(gLcdOps  == NULL)
			gLocateProtocol(MOD_LCD, (void*)&gLcdOps);
	
		if(gLcdOps != NULL)
		{
			printf("window->startPoint.x = 0x%x\n", window->startPoint.x);
			printf("window->startPoint.y = 0x%x\n", window->startPoint.y);
			printf("window->width = 0x%x\n", window->width);
			printf("window->height = 0x%x\n", window->height);
			gLcdOps->LcdDisplayStringWithColor((window->startPoint.x + x), (window->startPoint.y + y), text, fg);
		}
	}
}

int XuiCanvasDrawImg(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width,
	unsigned int height, XuiBgStyle bgstyle, XuiImg *img)
{
	if(window != NULL && img != NULL)
	{
		if(gLcdOps  == NULL)
			gLocateProtocol(MOD_LCD, (void*)&gLcdOps);
	
		if(gLcdOps != NULL)
		{
			printf("window->startPoint.x = 0x%x\n", window->startPoint.x);
			printf("window->startPoint.y = 0x%x\n", window->startPoint.y);
			printf("window->width = 0x%x\n", window->width);
			printf("window->height = 0x%x\n", window->height);
			gLcdOps->LcdDrawImage(x, y, (width + x), (height + y), img->priv);
		}
	}
}
	
int XuiCanvasDrawRect(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width,
	unsigned int height, XuiColor fg, int round, int fill);
	
int XuiClearArea(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, 
	unsigned int height)
{
	if(window != NULL)
	{
		if(gLcdOps  == NULL)
			gLocateProtocol(MOD_LCD, (void*)&gLcdOps);
	
		if(gLcdOps != NULL)
		{
			printf("window->startPoint.x = 0x%x\n", window->startPoint.x);
			printf("window->startPoint.y = 0x%x\n", window->startPoint.y);
			printf("window->width = 0x%x\n", window->width);
			printf("window->height = 0x%x\n", window->height);
			gLcdOps->LcdClearScreen(0x00);
		}
	}
}
	
int XuiTextWidth(XuiFont *font, int size, char *text);
	
XuiWindow *XuiCreateCanvas(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width,
	unsigned int height);
	
void XuiCanvasMoveToY (XuiWindow * window, unsigned int my);

void XuiDestroyWindow(XuiWindow *window)
{
	if(window != NULL)
	{
		free(window);
		window = NULL;
	}
}

void XuiShowWindow(XuiWindow *window, int show, int flag);

void XuiCanvasSetBackground(XuiWindow *window, XuiBgStyle bgstyle, XuiImg *img, XuiColor bg)
{
	printf("window = 0x%x\n", window);
	printf("bg = 0x%x\n", bg);
	if(window != NULL)
	{
		if(gLcdOps  == NULL)
			gLocateProtocol(MOD_LCD, (void*)&gLcdOps);
	
		if(gLcdOps != NULL)
		{
			printf("window->startPoint.x = 0x%x\n", window->startPoint.x);
			printf("window->startPoint.y = 0x%x\n", window->startPoint.y);
			printf("window->width = 0x%x\n", window->width);
			printf("window->height = 0x%x\n", window->height);
			gLcdOps->LcdDrawBox(window->startPoint.x, window->startPoint.y, (window->startPoint.x + window->width), 
				(window->startPoint.y + window->height), bg);
		}
	}
}

XuiWindow *XuiCreateButton(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width,
	unsigned int height);
int XuiButtonSetStat(XuiWindow *window, XuiButtonStatType type, XuiButtonStat *stat);
int XuiButtonSetKey(XuiWindow *window, int key);


XuiWindow * XuiCreateSignatureBoard (XuiWindow *parent, unsigned int x, unsigned int y,
	unsigned int width, unsigned int height);

int XuiSigBoardSetStat (XuiWindow *window, XuiSigBoardStat* stat);

XuiImg * XuiSigBoardImg(XuiWindow *window);

XuiSignData* XuiSigBoardSignData(XuiWindow *window);

XuiWindow *XuiCreateGif (XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width,
	unsigned int height, const char* path);

int XuiHasKey(void);
int XuiGetKey(void);
void XuiClearKey(void);

XuiImg *XuiCaptureScreen(void);
XuiImg *XuiCaptureCanvas(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width,
	unsigned int height);
XuiImg *XuiImgLoadFromFile(const char *file);
XuiImg *XuiImgLoadFromMem(unsigned char *address, unsigned long length, int type);
int XuiImgSaveToFile(XuiImg *img, const char *file);
int XuiImgToRgba(XuiImg *img, const char *rgba);
int XuiImgTransform(XuiImg *img, XuiTransform transform);

XuiImg* XuiImgCompose(XuiImg* img1, XuiImg* img2, unsigned int rate1, unsigned int rate2, int type);
void XuiImgFree(XuiImg *img);
int XuiSetStatusbarIcon(int index, const char* path);
int XuiGetHzString(XuiImeAttr attr, char *outstr, unsigned int maxlen, unsigned int timeout);

int XuiGetString(XuiGetStrAttr attr, char *outstr, unsigned char mode, int minlen, int maxlen);

char * XuiBidiStrdup(const char *str);
int XuiCanvasAnimation(XuiWindow *front, XuiWindow *background, unsigned int front_rate, 
	unsigned int background_rate, int type);

int XuiGetGesture(XuiGesture* gesture);

int XuiSetGestureRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

void XuiClearGesture(void);

int XuiShowSoftKeyboard(int type, int show);

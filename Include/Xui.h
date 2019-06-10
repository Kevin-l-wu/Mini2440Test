#ifndef XUI_H_
#define XUI_H_

//Enum define

typedef enum _XuiColor
{
	XUI_COLOR_B = 0x003f,
	XUI_COLOR_G = 0x07c0,
	XUI_COLOR_R = 0xf800,
	XUI_COLOR_A = 0x0000,
} XuiColor;

typedef enum _XuiTransform
{
	XUI_ROTATE_0,
	XUI_ROTATE_90,
	XUI_ROTATE_180,
	XUI_ROTATE_270
} XuiTransform;

typedef enum _XuiButtonStatType
{
	XUI_BTN_NORMAL,
	XUI_BTN_PRESSED,
} XuiButtonStatType;

typedef enum _XuiBgStyle
{
	XUI_BG_NORMAL,
	XUI_BG_TILE,
	XUI_BG_CENTER,
	XUI_BG_FOUR_CORNER,
} XuiBgStyle;

typedef enum _XuiFontSet
{
	XUI_FONT_MONO,
	XUI_FONT_GREY,
} XuiFontSet;

typedef enum _XuiTextStyle
{
	XUI_TEXT_NORMAL,
	XUI_TEXT_BOLD,
	XUI_TEXT_ITALIC,
	XUI_TEXT_BOLD_ITALIC,
} XuiTextStyle;

typedef enum _XuiSigPenFlat
{
	XUI_SIG_FLAT,
	XUI_SIG_NORMAL,
} XuiSigPenFlat;

typedef enum _XuiWindowType
{
	XUI_WIN_CANVAS,
	XUI_WIN_BUTTON,
	XUI_WIN_GIF,
	XUI_WIN_SIGBOARD,
} XuiWindowType;


typedef enum _XuiShowMode
{
	XUI_SHOW_NORMAL,
	XUI_SHOW_MIRROR,
	XUI_SHOW_ALL,
} XuiShowMode;

typedef enum _XuiAnimationType
{
	XUI_TRANSLATION,
	XUI_POLL,
	XUI_SCALE,
} XuiAnimationType;

typedef enum _XuiGestrureType
{
	XUI_GESTURE_FLINGLEFT,
	XUI_GESTURE_FLINGRIGHT,
	XUI_GESTURE_FLINGUP,
	XUI_GESTURE_FLINGDOWN,
} XuiGestrureType;


//Struct define
typedef void XuiFont;

typedef struct _XuiPoint
{
	unsigned int x;
	unsigned int y;
} XuiPoint;

typedef struct _XuiWindow
{
	XuiPoint startPoint;
	unsigned int width;
	unsigned int height;
	void* widget;
	XuiWindowType type;
	unsigned int key;
} XuiWindow;

typedef struct _XuiImg
{
	unsigned int width;
	unsigned int height;
	void* priv;
} XuiImg;

typedef struct _XuiButtonStat
{
	unsigned char btmRound;
	unsigned short btnBg;
	char text[10];
} XuiButtonStat;

typedef struct _XuiSigBoardStat
{
	unsigned char btmRound;
	unsigned short btnBg;
} XuiSigBoardStat;

typedef struct _XuiImeAttr
{
	XuiWindow* parent;
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
} XuiImeAttr;

typedef struct _XuiGetStrAttr
{
	XuiWindow* parent;
	unsigned int x;
	unsigned int y;
} XuiGetStrAttr;

typedef struct _XuiSignPoint
{
	unsigned int x;
	unsigned int y;
} XuiSignPoint;

typedef struct _XuiSignData
{
	XuiSignPoint pointArray[100];
	unsigned int pointLen;
} XuiSignData;

typedef struct _XuiGesture
{
	XuiGestrureType type;
	unsigned distance;
} XuiGesture;



int XuiOpen(int argc, char** argv);
int XuiIsRunning();
void XuiClose();
int XuiSuspend();
int XuiResume();

XuiWindow* XuiRootCanvas();
XuiWindow* XuiStatusbarCanvas();

XuiFont* XuiCreateFont(char* fontFile, int index, XuiFontSet fontSet);
void XuiDestroyFont(XuiFont* font);

int XuiCanvasDrawText(XuiWindow *window, unsigned int x, unsigned int y, unsigned int height,
	XuiFont *font, XuiTextStyle textstyle, XuiColor fg, char *text);

int XuiCanvasDrawImg(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width,
	unsigned int height, XuiBgStyle bgstyle, XuiImg *img);
	
int XuiCanvasDrawRect(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width,
	unsigned int height, XuiColor fg, int round, int fill);
	
int XuiClearArea(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, 
	unsigned int height);
	
int XuiTextWidth(XuiFont *font, int size, char *text);
	
XuiWindow *XuiCreateCanvas(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width,
	unsigned int height);
	
void XuiCanvasMoveToY (XuiWindow * window, unsigned int my);

void XuiDestroyWindow(XuiWindow *window);

void XuiShowWindow(XuiWindow *window, int show, int flag);

void XuiCanvasSetBackground(XuiWindow *window, XuiBgStyle bgstyle, XuiImg *img, XuiColor bg);

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




#endif
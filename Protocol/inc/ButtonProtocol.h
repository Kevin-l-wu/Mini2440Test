#ifndef _BUTTON_PROTOCOL_H_
#define _BUTTON_PROTOCOL_H_

#include "GlobalDefine.h"

typedef void (* BUTTON_INIT)();

typedef void (* BUTTON_EXIT)();

typedef int (* BUTTON_HIT)();

typedef void (* BUTTON_FLUSH)();

typedef int (* BUTTON_GET)();

typedef struct _ButtonModOps
{
	BUTTON_INIT ButtonInit;
	BUTTON_EXIT ButtonExit;
	BUTTON_HIT ButtonHit;
	BUTTON_FLUSH ButtonFlush;
	BUTTON_GET ButtonGet;
} ButtonModOps;


#endif
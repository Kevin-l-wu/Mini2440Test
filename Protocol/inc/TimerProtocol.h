#ifndef _NAND_PROTOCOL_H_
#define _NAND_PROTOCOL_H_

#include "GlobalDefine.h"

#include "Timer.h"

//
// Protocol GUID
//

typedef void (*TIMER_DELAY_SECONDS)(TIMER timer, int seconds);

typedef void (*TIMER_DELAY_MS)(TIMER timer, int ms);

typedef int (*IS_TIMEOUT)(TIMER timer);

typedef void (*TIMER_START_STOP)(TIMER timer, unsigned char isStart);

typedef struct _TimerModOps
{
	int* pTimer4IntCount;
	TIMER_DELAY_SECONDS TimerDelaySeconds;
	TIMER_DELAY_MS TimerDelayMs;
	IS_TIMEOUT IsTimeout;
	TIMER_START_STOP TimerStartStop;
} TimerModOps;

#endif	// _NAND_PROTOCOL_H_


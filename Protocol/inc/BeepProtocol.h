#ifndef BEEPPROTOCOL_H_
#define BEEPPROTOCOL_H_

typedef void (*BEEP_INIT)();

typedef void (*BEEP_ENABLE)();

typedef void (*BEEP_DISABLE)();

typedef struct _BeepModOps
{
	BEEP_INIT BeepInit;
	BEEP_ENABLE BeepEnable;
	BEEP_DISABLE BeepDisable;
} BeepModOps;


#endif
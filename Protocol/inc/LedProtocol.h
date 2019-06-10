#ifndef _LED_PROTOCOL_H_
#define _LED_PROTOCOL_H_

#include "GlobalDefine.h"


#define LED_MAX_NUMBER 4

//
// Protocol GUID
//

typedef void (*LED_INIT)();

typedef void (*LED_PRINT_CHAR)(char value);

typedef void (*LIGHT_LED)(int led_number);

typedef void (*PUT_OUT_LED)(int led_number);

typedef struct _LedModOps
{
	LED_INIT LedModInit;
	LED_PRINT_CHAR LedPrintChar;
	LIGHT_LED LightLed;
	PUT_OUT_LED PutOutLed;
} LedModOps;


#endif	// _LED_PROTOCOL_H_


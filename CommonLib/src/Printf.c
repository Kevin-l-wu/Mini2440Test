//#include <stdarg.h>
#include "Printf.h"

//static char gOutputBuf[PRINTF_MAX_OUTPUT];

int Printf(const char* fmt, ...)
{
/*	int index = 0;
	int length = 0;
	va_list args;
	va_start(args, fmt);
	
	length = vsLogPrintf_string((char*)gOutputBuf, fmt, args);
	
	va_end(args);
	
	for(index = 0; index < length; index++)
	{
		uart0_send_data(gOutputBuf[index]);
	}

	return length;
*/	
}

int sLogPrintf_string(char* str, char* format, ... )
{
	
}

int vsLogPrintf_string (char * s, const char * format, va_list arg )
{
	
}

int atoi (const char* str)
{
	
}

long int atol(const char* str )
{
	
}

int isdigit(int c)
{
	
}

int toupper(int c)
{
	
}

int tolower(int c)
{
	
}

void srand(unsigned int seed)
{
	
}

int rand(void)
{
	
}
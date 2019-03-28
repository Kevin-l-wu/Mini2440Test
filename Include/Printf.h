#ifndef PRINTF_H_
#define PRINTF_H_

#include <stdarg.h>

#define PRINTF_MAX_OUTPUT 30


int Printf(const char* fmt, ...);

int sprintf_string(char* str, char* format, ... );

int vsprintf_string (char * s, const char * format, va_list arg );

int atoi(const char * str);

long int atol ( const char * str );

int isdigit(int c);

int toupper(int c);

int tolower(int c);

void srand(unsigned int seed);

int rand(void);

#endif
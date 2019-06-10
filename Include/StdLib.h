/*
 * Copyright (c) 2011, Apple Inc.
 * All rights reserved.
 * LibC-lite - libc -> EFI glue library
 */
#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "GlobalDefine.h"

/*
#include <Tiano.h>
#include <LibCLite/efi_compat.h>
#include <LibCLite/setjmp.h>
*/
#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0

char *getenv(const char *name);

int atexit(void (*)(void));
void exit(int) __attribute__((noreturn));
void abort(void) __attribute__((noreturn));

void free(void *ptr);
void *realloc(void *ptr, size_t size);
void *malloc(size_t size);
void *calloc(size_t count, size_t size);
int system(const char *string);

typedef unsigned int time_t;
struct tm {
	int  tm_sec;    /* seconds after the minute [0-60] */
	int  tm_min;    /* minutes after the hour [0-59] */
	int  tm_hour;   /* hours since midnight [0-23] */
	int  tm_mday;   /* day of the month [1-31] */
	int  tm_mon;    /* months since January [0-11] */
	int  tm_year;   /* years since 1900 */
	int  tm_wday;   /* days since Sunday [0-6] */
	int  tm_yday;   /* days since January 1 [0-365] */
	int  tm_isdst;  /* Daylight Savings Time flag */
	long tm_gmtoff; /* offset from CUT in seconds */
	char *tm_zone;  /* timezone abbreviation */
};

struct timeval {
    int tv_sec;
    int tv_usec;
};

struct timespec {
	int tv_sec;
	int tv_nsec;
};

int difftimespec(struct timespec tv2, struct timespec tv1, struct timespec *diff);

time_t time(time_t *tloc);
time_t mktime(struct tm *timeptr);
struct tm *localtime(const time_t *timer);
struct tm *gmtime(const time_t *timer);
size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *timptr);

long atol(const char *str);
double strtod(const char * nptr, char ** endptr);

/* Skip any leading whitespace and parse the sign.
 * If base is 0, check 0x for hex, 0b for binary, and 0 for octal
 * else assume decimal.
 * If base is already 16, allow 0x
 * If base is already 2,  allow 0b
 */
long strtol(const char *str, char **endptr, int base);

/* This would be INT_MAX if we had that definition vailable */
#define RAND_MAX (~0U >> 1)

int rand(void);
void srand(unsigned int seed);

void __stack_chk_guard_setup(void);

void __stack_chk_fail(void);

#endif

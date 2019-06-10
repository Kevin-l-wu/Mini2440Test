/*
 * Copyright (c) 2011, Apple Inc.
 * All rights reserved.
 * LibC-lite - libc -> EFI glue library
 */
#ifndef __STRING_H__
#define __STRING_H__


#include "GlobalDefine.h"

// In EfiCommonLib
void* memcpy(void * s1, const void * s2, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void* memmove(void *s1, const void *s2, size_t n);
void* memset(void* s, int c, size_t n);
void bzero(void* s, size_t n);

/*
 * String
 */

size_t strlen(const char *s);
char *strerror(int errnum);
int strcmp(const char *s1, const char *s2);
int strncmp(const char * s1, const char * s2, size_t num);
char *strstr(const char *s1, const char *s2);
char *strchr(const char *s, int c);
unsigned long strtoul(const char * str, char ** endptr, int base);

int stricmp(const char *s1, const char *s2);
int strnicmp(const char *s1, const char *s2, size_t n);
#define strcasecmp stricmp

char *strrchr(const char *s, int c);
char *strncpy(char * s1, const char * s2, size_t n);
size_t strlcpy(char *dst, const char *src, size_t size);
size_t strlcat(char *dst, const char *src, size_t size);
char *strcpy(char * s1, const char * s2);
char *strcat(char * s1, const char * s2);
char *strncat(char * s1, const char * s2, size_t n);
size_t strcspn(const char *s1, const char *s2);
size_t strspn(const char *s1, const char *s2);
int putchar(int c);
char *strpbrk(const char *s1, const char *s2);
#define strcoll(s1, s2) strcmp(s1, s2)
void *memchr(const void *s, int c, size_t n);

#endif

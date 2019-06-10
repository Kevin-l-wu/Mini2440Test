/*
 * Copyright (c) 2011, Apple Inc.
 * All rights reserved.
 * LibC-lite - libc -> EFI glue library
 */
#ifndef __CTYPE_H__
#define __CTYPE_H__

int tolower(int c);
int toupper(int c);
int isspace(int c);
int iscntrl(int c);
int isdigit(int c);
int isalnum(int c);
int isalpha(int c);
int isprint(int c);
int isgraph(int c);
int islower(int c);
int isupper(int c);
int ispunct(int c);
int isxdigit(int c);

#endif

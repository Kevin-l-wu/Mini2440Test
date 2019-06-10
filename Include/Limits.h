/*
 * Copyright (c) 2011, Apple Inc.
 * All rights reserved.
 * LibC-lite - libc -> EFI glue library
 */
#ifndef __LIMITS_H__
#define __LIMITS_H__

#define UCHAR_MAX (255)
#define CHAR_MAX  (127)
#define CHAR_MIN  (-128)

#if (EFI_MAX_BIT > 0x80000000)
#define LONG_MAX 9223372036854775807LL
#else
#define LONG_MAX 2147483647L
#endif
#define LONG_MIN (-LONG_MAX-1)

#define SHRT_MAX  (32767)
#define USHRT_MAX (SHRT_MAX *2  +1)

#endif

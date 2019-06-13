/*
 * Copyright (c) 2011, Apple Inc.
 * All rights reserved.
 * LibC-lite - libc -> EFI glue library
 */
#ifndef __STDIO_H__
#define __STDIO_H__

#include "GlobalDefine.h"


struct file_s;
typedef struct file_s FILE;

/*
 * Streams
 */
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#define BUFSIZ  1024        /* size of buffer used by setbuf */
#define EOF (-1)

int fprintf(FILE * stream, const char * format, ...);
FILE *fopen(const char * path, const char * mode);
FILE *freopen(const char * filename, const char * mode,
         FILE * stream);
int fclose(FILE *stream);
int fflush(FILE *stream);
int fscanf(FILE * stream, const char * format, ...);
int ferror(FILE *stream);
int getc(FILE *stream);
int ungetc(int c, FILE *stream);
char *fgets(char * s, int n, FILE * stream);
size_t fread(void * ptr, size_t size, size_t nitems, FILE * stream);
size_t fwrite(const void * ptr, size_t size, size_t nitems, FILE * stream);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
int feof(FILE *stream);
int fputs(const char * s, FILE * stream);
FILE *tmpfile(void);
void clearerr(FILE *stream);
int snprintf(char * s, size_t size, const char * format, ...);
int sprintf(char * s, const char * format, ...);
int printf(const char * format, ...);
int vsnprintf(char * s, size_t size, const char * format, VA_LIST ap);
int vsprintf(char * s, const char * format, VA_LIST ap);
int vprintf(const char * format, VA_LIST ap);

#ifndef SEEK_SET
#define SEEK_SET    EfiSeekStart   /* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define SEEK_CUR    EfiSeekCurrent   /* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define SEEK_END    EfiSeekEnd   /* set file offset to EOF plus offset */
#endif

#endif

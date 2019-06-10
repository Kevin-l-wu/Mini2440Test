
//#include "efi_compat.h"
#include "Stdio.h"
//#include <EfiPrintLib.h>
//#include <EfiFileLib/EfiFileLib.h>
#include "Errno.h"
#include "String.h"
#include "SimpleFile.h"
#include "CoreMalloc.h"
#include "Print.h"
#include "StdArg.h"
//#include "Uart.h"


#define LogDebug printf_string

#define FPRINTF_BUF_SIZE	8192

// File internals
enum {
	kFileTypeTmp = 0,
	kFileTypeEfiOpenFile,
	kFileTypeStdin,
	kFileTypeStdout,
	kFileTypeStderr,
};

struct tmp_file_t {
	VOID *ptr;
	UINTN cursor;
	UINTN size;
};

typedef size_t (*f_write_ptr)(const void * ptr, size_t size, size_t nitems, FILE * stream);
typedef size_t (*f_read_ptr)(void * ptr, size_t size, size_t nitems, FILE * stream);
typedef int (*f_eof_ptr)(FILE *stream);
typedef int (*f_seek_ptr)(FILE *stream, long offset, int whence);
typedef long (*f_tell_ptr)(FILE *stream);
typedef int (*vf_printf_ptr)(FILE * stream, const char * efi_format, VA_LIST args);
typedef int (*f_flush_ptr)(FILE *stream);
typedef int (*getc_ptr)(FILE *stream);
typedef int (*f_puts_ptr)(const char * s, FILE * stream);

typedef struct {
	f_write_ptr		fwrite;
	f_read_ptr		fread;
	f_eof_ptr		feof;
	f_seek_ptr		fseek;
	f_tell_ptr		ftell;
	vf_printf_ptr	vfprintf;
	f_flush_ptr		fflush;
	getc_ptr		getc;
	f_puts_ptr		fputs;
} file_opts_t;

struct file_s {
	int type;
	union {
		// kFileTypeTmp
		struct tmp_file_t *tmp;
		// kFileTypeEfiOpenFile
		EFI_OPEN_FILE *fp;
	};
	file_opts_t ops;
	int err_no;
	int ungetc;
};


char *__fixup_format_string(char * format)
{
	//TODO	
}

VOID *
EfiLibAllocateCopyPool (
	IN  UINTN   AllocationSize,
	IN  VOID    *Buffer
	)
{
	//TODO  
}


static int __vsprintf_i (char ** buf, const char * efi_format, VA_LIST ap, int minsize) {
	static char *printbuf = NULL;
	static size_t printbufsize = 0;
	int r = 0;
	if (minsize > 0) {
		if (printbufsize < minsize) {
			printbufsize = minsize;
			if (printbuf) {
				CoreFree(printbuf);
				printbuf = NULL;
			}
		}
		while (1)
		{
			if (!printbuf) {
				printbuf = (char*)CoreMalloc(printbufsize);
				if (!printbuf) {
					printbufsize = 0;
					r = -(errno = ENOMEM);
					break;
				}
			}
			
			// NOTE: Returned size does not include terminator
			r = (int)AsciiVSPrint(printbuf, printbufsize, efi_format, ap);
			
			// Retry on the possibility that the output was truncated
			if (r < printbufsize - 1) {
				break;
			} else {
				printbufsize += minsize;
				CoreFree(printbuf);
				printbuf = NULL;
			}
		}
	} else {
		r = -(errno = EINVAL);
	}
	if (buf) {
		*buf = printbuf;
	}
	return r;
}

size_t null_fread(void * ptr, size_t size, size_t nitems, FILE * stream) {
	(void)ptr;
	(void)size;
	(void)nitems;
	stream->err_no = EOF;
	
	return 0;
}

size_t null_fwrite(const void * ptr, size_t size, size_t nitems,
         FILE * stream) {
	(void)ptr;
	(void)size;
	(void)nitems;
	stream->err_no = EOF;
	
	return 0;
}

int null_feof(FILE * stream) {
	(void)stream;
	stream->err_no = 0;
	
	return 0;
}

int null_fseek(FILE *stream, long offset, int whence) {
	(void)stream;
	(void)offset;
	(void)whence;
	stream->err_no = EOF;
	
	errno = EINVAL;
	return -1;
}

long null_ftell(FILE *stream) {
	(void)stream;
	stream->err_no = errno = ESPIPE;
	return -1;
}

int null_vfprintf(FILE * stream, const char * efi_format, VA_LIST args) {
	(void)stream;
	(void)efi_format;
	(void)args;
	stream->err_no = EOF;
	
	return EOF;
}

int null_fflush(FILE *stream) {
	(void)stream;
	
	switch(stream->type) {
		case kFileTypeStdout:
		case kFileTypeStderr:
			return 0;
		case kFileTypeStdin:
			return EBADF;
		default:
			break;
	}
	
	return 0;
}

int null_getc(FILE *stream) {
	stream->err_no = EOF;
	return EOF;
}

int null_fputs(const char * s, FILE * stream) {
	stream->err_no = EOF;
	return EOF;
}

size_t stdin_fread(void * ptr, size_t size, size_t nitems, FILE * stream) {
	size_t ncount = 0;
	char *cptr = (char*)ptr;
	/*
	if(stdin == stream) {
		//LogDebug("reading stdin");
		while(EFI_SUCCESS == gBS->CheckEvent(gST->ConIn->WaitForKey)) {
			EFI_INPUT_KEY Key;
			if(EFI_SUCCESS == gST->ConIn->ReadKeyStroke(gST->ConIn, &Key)) {
				*cptr++ = (char)(Key.UnicodeChar & 0xff);
				ncount++;
			}
		}
	}
	*/
	return ncount;
}

int stdin_getc(FILE *stream) {
	EFI_STATUS ret;
	UINTN idx = 0;
/*	
	ret = gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &idx);
	if(EFI_SUCCESS == ret) {
		EFI_INPUT_KEY Key;
	
		ret = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
		if(EFI_SUCCESS == ret) {
			//LogDebug("Unicode: 0x%x ScanCode: 0x%x", Key.UnicodeChar, Key.ScanCode);
			int c = (int)((unsigned int)((char)(Key.UnicodeChar & 0xff)));
			if(0x04 == c) return EOF;	// Ctrl-D == EOF on stdin
			if('\r' == c) {
				// convert \r to \n
				c = '\n';
			}
			
			// Echo on
			printf_string("%c", c);
			
			return c;
		}
	}
	
	return EOF;
*/
}

size_t stdout_fwrite(const void * ptr, size_t size, size_t nitems, FILE * stream) {
	// None of this would really be necessary if we had this:
	// <rdar://problem/16422770> [Console] Console Re-arch needs to handle raw data with consolemux's routings
	static char *printbuf = NULL;
	static size_t printbufsize = 0;
	size_t nbytes, nprinted;
	size_t total = size * nitems;
	char *p, *e, *termbuf, *termbuf_end;

	if (total == 0) {
		return 0;
	}

	// Make sure the buffer is always null-terminated
	// NOTE: This helps avoids using printf_string("%c") for Lua-style strings
	//       (which do not have a null terminator)
	if (((char *)ptr)[total - 1] == 0) {
		termbuf = (char *)ptr;
	} else {
		nbytes = total + 1;
		if (printbuf && (printbufsize < nbytes)) {
			CoreFree(printbuf);
			printbuf = NULL;
			printbufsize = 0;
		}
		if (!printbuf) {
			printbuf = (char*)CoreMalloc(nbytes);
			if (!printbuf) {
				return -(errno = ENOMEM);
			}
			printbufsize = nbytes;
		}

		memcpy(printbuf, (void *)ptr, total);
		printbuf[total] = 0;

		termbuf = printbuf;
	}

	// Print each null-terminated substring
	p = termbuf;
	termbuf_end = termbuf + total;
	while (p < termbuf_end) {
		// Find the end of the current substring
		for (e = p; (e < termbuf_end) && *e; ++e);
		nbytes = e - p;
		if (nbytes == 0) {
			nbytes = 1;
			nprinted = printf_string("%c", *p);
		} else {
			nprinted = printf_string("%a", p);
		}
		p += nprinted;
		if (nprinted != nbytes) {
			break;
		}
	}

	return (p - termbuf) / size;
}

int stdout_fputs(const char * s, FILE * stream) {
	(void)stream;
	
	printf_string("%a", s);
	return 0;
}

size_t openfile_fwrite(const void * ptr, size_t size, size_t nitems, FILE * stream) {
	//
	
	UINTN items_written = 0;
	char* cptr = (char*)ptr;
	EFI_STATUS ret;
	
	if(!stream || !stream->fp || !stream->fp->FsFileInfo || (stream->fp->FsFileInfo->Attribute & EFI_FILE_DIRECTORY)) {
		// When writing to directories, EISDIR seems acceptable, but EBADF matches Mac OS
		errno = EBADF;
		if(stream) { stream->err_no = errno; }
		return 0;
	}

	if(!size || !nitems) {
		return 0;
	}
	
	EFI_OPEN_FILE *f = stream->fp;
	size_t orig_size = size;
	
	do {
		ret = EfiWrite(f, (void*)cptr, &size);
		
		if(size != orig_size || EFI_ERROR(ret)) {
			break;
		}
		
		cptr += size;
		items_written++;
	} while(items_written < nitems);
	
	return items_written;
}

size_t openfile_fread(void * ptr, size_t size, size_t nitems, FILE * stream) {
	
	
	int code = 0;
	if(!stream || !stream->fp || !stream->fp->FsFileInfo) {
		code = EBADF;
	} else if(stream->fp->FsFileInfo->Attribute & EFI_FILE_DIRECTORY) {
		// Matches Mac OS behavior
		code = EISDIR;
	}
	if(code) {
		errno = code;
		if(stream) { stream->err_no = errno; }
		return 0;
	}
		
	EFI_OPEN_FILE *f = stream->fp;
	UINTN f_nitems = (f->FsFileInfo->FileSize - f->CurrentPosition) / size;
	UINTN count = EFI_MIN(nitems, f_nitems);

	EFI_STATUS ret = EfiRead(f, ptr, size * count);
	if(EFI_ERROR(ret)) {
		return 0;
	}

	if((f->CurrentPosition + size > f->FsFileInfo->FileSize) && (nitems > count)) {
		stream->err_no = EOF;
	}
	
	return count;
}

int openfile_fseek(FILE *stream, long offset, int whence) {
	EFI_LBA lba = (EFI_LBA)offset;
	EFI_SEEK_TYPE seek = (EFI_SEEK_TYPE)whence;
	
	if(!stream->fp || !stream->fp->FsFileInfo) {
		errno = EBADF;
		return -1;
	}
	
	// Seek from the current offset (should be 0)
	if(EfiSeekEnd == whence) {
		if(offset < 0) {
			lba = (long)stream->fp->FsFileInfo->FileSize + offset;
			seek = EfiSeekStart;
		} else if(offset == 0) {
			lba = 0;
			seek = EfiSeekEnd;
		} else {
			errno = EINVAL;
			return -1;
		}
	}
	
	EFI_STATUS result = EfiSeek(stream->fp, lba, seek);
	switch(result) {
		case EFI_SUCCESS:
			break;
		case EFI_UNSUPPORTED:
			errno = EBADF;
			return -1;
		case EFI_NOT_FOUND:
		default:
			// bounds error
			errno = EINVAL;
			return -1;
		
	}
	
	return 0;
}

long openfile_ftell(FILE *stream) {
	EFI_LBA offset = 0ULL;
	if(!stream || !stream->fp) {
		errno = EBADF;
		return -1;
	}
	
	(void)EfiTell(stream->fp, &offset);
	return (long)offset;
}

int generic_vfprintf(FILE * stream, const char * efi_format, VA_LIST args) {
	char *buf;
	int r = __vsprintf_i(&buf, efi_format, args, FPRINTF_BUF_SIZE);
	if (r < 0) { return r; }
	return (int)fwrite(buf, 1, r, stream);	
}

int openfile_getc(FILE *stream) {
	int code = 0;
	if(!stream || !stream->fp || !stream->fp->FsFileInfo) {
		code = EBADF;
	} else if(stream->fp->FsFileInfo->Attribute & EFI_FILE_DIRECTORY) {
		// Matches Mac OS behavior
		code = EISDIR;
	}
	if(code) {
		errno = code;
		if(stream) { stream->err_no = errno; }
		return EOF;
	}
	
	int c = 0;
	int count = (int)fread(&c, 1, 1, stream);
	if(count == 1) {
		return c;
	}
	stream->err_no = EOF;
	return EOF;
}

int openfile_fputs(const char * s, FILE * stream) {
	if(!stream || !stream->fp || !stream->fp->FsFileInfo || (stream->fp->FsFileInfo->Attribute & EFI_FILE_DIRECTORY)) {
		// When writing to directories, EISDIR seems acceptable, but EBADF matches Mac OS
		errno = EBADF;
		if(stream) { stream->err_no = errno; }
		return EOF;
	}
	
	size_t len = strlen((CHAR8*)s);
	return (len == fwrite(s, 1, len, stream)) ? 0 : EOF;
}

int openfile_feof(FILE * stream) {
	if(!stream || !stream->fp) return 0;
	
	EFI_LBA curPos = 0;
	UINTN size = EfiTell(stream->fp, &curPos);
	
	if(curPos == (EFI_LBA)size) {
		stream->err_no = EOF;
	}
	
	return (stream->err_no == EOF);
}

static struct file_s stdin_s = {
	.type = kFileTypeStdin,
//	.fp = NULL,
	.ops = {
		.fwrite = null_fwrite,
		.fread = stdin_fread,
		.feof = null_feof,
		.fseek = null_fseek,
		.ftell = null_ftell,
		.vfprintf = null_vfprintf,
		.fflush = null_fflush,
		.getc = stdin_getc,
		.fputs = null_fputs,
	},
	.err_no = 0,
	.ungetc = -1,
};

static struct file_s stdout_s = {
	.type = kFileTypeStdout,
//	.fp = NULL,
	.ops = {
		.fwrite = stdout_fwrite,
		.fread = null_fread,
		.feof = null_feof,
		.fseek = null_fseek,
		.ftell = null_ftell,
		.vfprintf = generic_vfprintf,
		.fflush = null_fflush,
		.getc = null_getc,
		.fputs = stdout_fputs,
	},
	.err_no = 0,
	.ungetc = -1,
};

static struct file_s stderr_s = {
	.type = kFileTypeStderr,
//	.fp = NULL,
	.ops = {
		.fwrite = stdout_fwrite,
		.fread = null_fread,
		.feof = null_feof,
		.fseek = null_fseek,
		.ftell = null_ftell,
		.vfprintf = generic_vfprintf,
		.fflush = null_fflush,
		.getc = null_getc,
		.fputs = stdout_fputs,
	},
	.err_no = 0,
	.ungetc = -1,
};

static struct file_s openfile_template = {
	.type = kFileTypeEfiOpenFile,
//	.fp = NULL,
	.ops = {
		.fwrite = openfile_fwrite,
		.fread = openfile_fread,
		.feof = openfile_feof,
		.fseek = openfile_fseek,
		.ftell = openfile_ftell,
		.vfprintf = generic_vfprintf,
		.fflush = null_fflush,
		.getc = openfile_getc,
		.fputs = openfile_fputs,
	},
	.err_no = 0,
	.ungetc = -1,
};

FILE *stdin = &stdin_s;
FILE *stdout = &stdout_s;
FILE *stderr = &stderr_s;

int fprintf(FILE * stream, const char * format, ...) {	
	char *efi_format = __fixup_format_string((char*)format);
	
	if(!stream || !stream->ops.vfprintf) {
		errno = EINVAL;
		return -EINVAL;
	}
	
	if(!efi_format) {
		errno = ENOMEM;
		return -ENOMEM;
	}
	
	VA_LIST args;
	
	VA_START(args, format);
	int r = stream->ops.vfprintf(stream, efi_format, args);
	VA_END(args);
	
	CoreFree(efi_format);
	
	return r;
}

FILE *fopen(const char * path, const char * mode) {
	//LogDebug("%a(%a, %a)", __FUNCTION__, path, mode);

	UINT64 open_mode = 0;
	BOOLEAN failed = FALSE;
	
	struct file_s *file = EfiLibAllocateCopyPool(sizeof(struct file_s), (void*)&openfile_template);
	if(!file) {
		errno = ENOMEM;
		return NULL;
	}

	for ( ; *mode; mode++) {
		switch (*mode) {
			case 'r':
				open_mode |= EFI_FILE_MODE_READ;
				break;
			case 'w':
				open_mode |= EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE |
								EFI_FILE_MODE_TRUNCATE;
				break;
			case 'a':
				open_mode |= EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE |
								EFI_FILE_MODE_APPEND;
				break;
			case '+':
				open_mode |= EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE;
			default:
				// Ignores 'b' input
				break;
		}
	}

	file->fp = EfiOpen((CHAR8*)path, open_mode, 0);
	if (file->fp == NULL || file->fp->FsFileInfo == NULL) {
		LogDebug("file open failed");
		failed = TRUE;
		errno = ENOENT;
	}
	if ((open_mode & EFI_FILE_MODE_WRITE) && (file->fp->FsFileInfo->Attribute & EFI_FILE_DIRECTORY)) {
		LogDebug("failed to open directory as file for write");
		failed = TRUE;
		errno = EISDIR;
	}
	// File permission checks, if any, come after the directory/write check

	if (failed) {
		EfiClose(file->fp);
		CoreFree(file);
		return NULL;
	} else {
		return file;
	}
}

FILE *freopen(const char * filename, const char * mode,
         FILE * stream) {
	
	
	(void)fclose(stream);
	
	return fopen(filename, mode);
}

int fclose(FILE *stream) {
	
	
	if(!stream) {
		errno = EBADF;
		return EOF;
	}
	
	switch(stream->type) {
		case kFileTypeEfiOpenFile:
			if(stream->fp) {
				EfiClose(stream->fp);
				stream->fp = NULL;
			}
			break;
		case kFileTypeTmp:
			if(stream->tmp) {
				if(stream->tmp->ptr) {
					CoreFree(stream->tmp->ptr);
				}
				CoreFree(stream->tmp);
			}
			stream->tmp = NULL;
			break;
		case kFileTypeStderr:
		case kFileTypeStdin:
		case kFileTypeStdout:
			return 0;
	}
	
	CoreFree(stream);
	
	return 0;
}

int fflush(FILE *stream) {	
	// FIXME off spec, null stream should flush all streams
	if(!stream || !stream->ops.fflush) {
		errno = EBADF;
		return EOF;
	}
	return stream->ops.fflush(stream);
}

int fscanf(FILE * stream, const char * format, ...) {
	//LogDebug("fscanf(0x%x, %a, ...", stream, format);
	return EOF;
}

int ferror(FILE *stream) {
	if(stream->err_no) {
		switch(stream->err_no) {
			case EOF:
				// not an error in the sense of ferror
				return 0;
			default:
				return 1;
		}
	}
				
	return 0;
}

int getc(FILE *stream) {
	if(!stream || !stream->ops.getc) {
		errno = EBADF;
		return EOF;
	}
	
	// 1-char of ungetc supported
	if(-1 != stream->ungetc) {
		int c = stream->ungetc;
		stream->ungetc = -1;
		return c;
	}
	
	return stream->ops.getc(stream);
}

int ungetc(int c, FILE *stream) {	
	if(EOF == c) {
		return EOF;
	}
	
	if(-1 == stream->ungetc) {
		stream->ungetc = c;
		stream->err_no = 0;
		return c;
	}
	
	return EOF;
}

char *fgets(char * s, int n, FILE * stream) {
	int i;
	char *r = s;
	for(i = 0; i < n - 1; i++) {
		int c = getc(stream);
		if(EOF == c) {
			r = NULL;
			break;
		}
		s[i] = c;
		if(c == '\n' || c == '\r') {
			s[i++] = '\n';
			break;
		}
	}
	s[i] = 0;
	return r;
}

size_t fread(void * ptr, size_t size, size_t nitems, FILE * stream) {
	//LogDebug("%a(0x%x, %d, %d, 0x%x)", __FUNCTION__, ptr, size, nitems, stream);
	
	if(!stream || !stream->ops.fread) return 0;
	
	stream->ungetc = -1;
	
	return stream->ops.fread(ptr, size, nitems, stream);
}

size_t fwrite(const void * ptr, size_t size, size_t nitems, FILE * stream) {	
	if(!stream || !stream->ops.fwrite) return 0;
	
	stream->ungetc = -1;
	
	return stream->ops.fwrite(ptr, size, nitems, stream);
}

int fseek(FILE *stream, long offset, int whence) {	
	if(!stream || !stream->ops.fseek) {
		errno = EBADF;
		return -1;
	}
	
	stream->ungetc = -1;
	
	return stream->ops.fseek(stream, offset, whence);
}

long ftell(FILE *stream) {	
	if(!stream || !stream->ops.ftell) {
		errno = EBADF;
		return -1;
	}
	return stream->ops.ftell(stream);
}

int feof(FILE *stream) {
	if(!stream || !stream->ops.feof) return 0;
	return stream->ops.feof(stream);
}

int fputs(const char * s, FILE * stream) {
	if(!stream || !stream->ops.fputs) {
		errno = EBADF;
		return EOF;
	}
	
	stream->ungetc = -1;
	
	return stream->ops.fputs(s, stream);
}

FILE *tmpfile(void) {
	
	return NULL;
}

void clearerr(FILE *stream) {
	
	if(stream) {
		stream->err_no = 0;
	}
}

static int __printf_i(BOOLEAN silent, char * s, size_t size, const char * format, VA_LIST ap) {
//	printf_string("__printf_i(): format = %s\n", format);
//	printf_string("__printf_i(): va_args = %s\n", ap);
    
	if (silent && !s && size) {
		return -(errno = EINVAL);
	}
    
	// Switch %s to %a
	char *efi_format = __fixup_format_string((char *)format);
	if(!efi_format) {
		return -(errno = ENOMEM);
	}
    
	int r;
	char *buf;
    
	if (silent) {
		r = __vsprintf_i(&buf, efi_format, ap, 256);
        if (r >= 0) {
			if (s) {
				if (size == (size_t)(-1)) {
					strcpy(s, buf);
				} else if (size > 0) {
					strncpy(s, buf, EFI_MIN(r + 1, size));
					s[size - 1] = 0;
				}
			}
		}
	} else {
		AsciiPrintMarker(efi_format, ap);
//		printf_string(format, ap);
		r = 0;
	}
    
Exit:
	
	CoreFree(efi_format);
    
	return r;
}

int vsnprintf(char * s, size_t size, const char * format, VA_LIST ap) {
	
	return __printf_i(TRUE, s, size, format, ap);
}

int vsprintf(char * s, const char * format, VA_LIST ap) {
	
	return __printf_i(TRUE, s, (size_t)(-1), format, ap);
}

int vprintf(const char * format, VA_LIST ap) {
	
	return __printf_i(FALSE, NULL, (size_t)(-1), format, ap);
}

int snprintf(char * s, size_t size, const char * format, ...) {
	
    
	VA_LIST va_args;
	
	VA_START (va_args, format);
	int r = __printf_i(TRUE, s, size, format, va_args);
	VA_END (va_args);
	
	return r;
}

int sprintf(char * s, const char * format, ...) {
	
    
	VA_LIST va_args;
    
	VA_START (va_args, format);
	int r = __printf_i(TRUE, s, (size_t)(-1), format, va_args);
	VA_END (va_args);
    
	return r;
}

int printf(const char* format, ...) {
	VA_LIST va_args;
    
	VA_START (va_args, format);

	int r = __printf_i(FALSE, NULL, (size_t)(-1), format, va_args);
	VA_END (va_args);
    
	return r;
}

#include "String.h"
#include "Ctype.h"
#include "Errno.h"
#include "Stdio.h"

void* memcpy(void* s1, const void* s2, size_t n)
{
	char* su1;
	const char* su2;
	
	for(su1 = s1, su2 = s2; 0 < n; ++su1, ++su2, --n)
		*su1 = *su2;
	
	return (s1);
}

int memcmp(const void *s1, const void *s2, size_t n) {
	//TRACER();
	size_t ii;
	char *cs1 = (char*)s1;
	char *cs2 = (char*)s2;
	
	for(ii = 0; ii < n; ii++) {
		int diff = cs2[ii] - cs1[ii];
		if(0 != diff) return diff;
	}
	
	return 0;
}

void* memmove(void* dest, const void* src, unsigned int count)
{
	char* dstPtr = (char*)dest;
    char* srcPtr = (char*)src;
    
    if((dstPtr != NULL) && (srcPtr != NULL) && (count > 0))
    {
        if((srcPtr < dstPtr) && (dstPtr < (srcPtr + count)))
        {
            do
            {
                *(dstPtr + count - 1) = *(srcPtr + count - 1);
            }while(--count);
        }
        else
        {
            while(count--)
            {
                *dstPtr++ = *srcPtr++;
            }
        }
    }
    
    return dest;  
}

void* memset(void* s, int c, size_t n)
{
	const unsigned char uc = c;
	unsigned char* su;
	
	for(su = s; 0 < n; ++su, --n)
	{
		*su = uc;
	}
	
	return (s);
}

void bzero(void* s, size_t n)
{
	memset(s, 0, n);
}

int tolower(int c) {
	if(isupper(c)) {
		return 'a' + (c - 'A');
	}
	return c;
}

int toupper(int c) {
	if(islower(c)) {
		return 'A' + (c - 'a');
	}
	return c;
}

int _isinset(int c, char *x, int sz) {
	int ii;
	for(ii = 0; ii < sz; ii++) {
		if(c == x[ii]) return 1;
	}
	return 0;
}

int isspace(int c) {
	static char x[] = {' ', '\t', '\v', '\n', '\f'};
	return _isinset(c, x, countof(x));
}

int iscntrl(int c) {	
	return ((c >= 0 && c <= 31) || c == 127);
}

int isdigit(int c) {
	return (c <= '9' && c >= '0');
}

int isalnum(int c) {
	return (isalpha(c) || isdigit(c));
}

int isalpha(int c) {
	int cl = tolower(c);
	return (cl >= 'a' && cl <= 'z');
}

int isprint(int c) {
	return (isalnum(c) || ispunct(c) || c == ' ');
}

int isgraph(int c) {
	return (33 <= c && c <= 126);
}

int islower(int c) {
	return (c >= 'a' && c <= 'z');
}

int isupper(int c) {
	return (c >= 'A' && c <= 'Z');
}

int ispunct(int c) {
	static char x[] = {
		'!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*',
		'+', ',', '-', '.', '/', ':', ';', '<', '=', '>',
		'?', '@', '[', ']', '\\', '^', '_', '`', '{', '|',
		'}', '~'
	};
	return _isinset(c, x, countof(x));
}

int isxdigit(int c) {
	int cl = tolower(c);
	return (isdigit(cl) || (cl >= 'a' && cl <= 'f'));
}

size_t strlen(const char *s) {
	const char* sc;
	
	for(sc = s; *sc != 0; ++sc)
		;
	
	return (sc - s);
}

int strcmp(const char* s1, const char* s2) {
	for(; *s1 == *s2; ++s1, ++s2)
	{
		if(*s1 == '\0')
			return 0;
	}
	
	return (*((unsigned char *)s1) < *((unsigned char*)s2) ? -1 : 1);
}

int strncmp(const char* s1, const char* s2, size_t n) {
	for(; 0 < n; ++s1, ++s2, --n)
	{
		if(*s1 != *s2)
			return (*((unsigned char *)s1) < *((unsigned char*)s2) ? -1 : 1);
		else if(*s1 == '\0')
			return 0;
	}
	
	return 0;
}
int stricmp(const char *s1, const char *s2) {
	//return (int)AsciiStriCmp((char *)s1, (char *)s2);
}

int strnicmp(const char *s1, const char *s2, size_t n)
{
	
}

char *strstr(const char *s1, const char *s2) {
	if(*s2 == '\0')
		return ((char*)s1);
	
	for(; (s1 = strchr(s1, *s2)) != NULL; ++s1)
	{
		char* sc1, * sc2;
		
		for(sc1 = (char*)s1, sc2 = (char*)s2;;)
		{
			sc2++;
			sc1++;
			if(*(sc2) == '\0')
				return (char*)s1;
			else if(*(sc1) != *sc2)
				break;
		}
	}
	
	return NULL;
}

char *strchr(const char *s, int c) {
	do {
		if(*s == c) return (char*)s;
	}while(*s++);
	
	return NULL;
}

unsigned long strtoul(const char *nptr, char **endptr, int base) {
	unsigned long rval = 0, digit, cusp, limit;
	const char *str = nptr, *significand;
	char reference;
	int negative = 0, overflow = 0;
	
	if (nptr == NULL) {
		goto InvalidNumber;
	}

	// Bypass white space (will automatically stop at null)
	while (isspace(*str)) {
		str++;
	}

	// Handle negatives (part 1)
	if (*str == '-') {
		negative = 1;
		++str;
	} else if (*str == '+') {
		negative = 0;
		++str;
	}

	// Radix handling
	if (base == 0) {
		if (*str == '0') {
			if ((str[1] == 'X') || (str[1] == 'x')) {
				base = 16;
				str += 2;
			} else {
				base = 8;
			}
		} else {
			base = 10;
		}
	} else if ((base == 16) && (*str == '0') && ((str[1] == 'X') || (str[1] == 'x'))) {
		str += 2;
	}
	if ((base < 2) || (36 < base)) {
		goto InvalidNumber;
	}

	// Overflow handling
	cusp = (unsigned long)(-1LL) / base;
	limit = (unsigned long)(-1LL) - (cusp * base);

	// Convert according to base and respect overflow
	significand = str;
	for (; *str; ++str) {
		// Convert from character to digit
		if (isdigit(*str)) {
			reference = '0';
		} else if (islower(*str)) {
			reference = 'a' - 10;
		} else if (isupper(*str)) {
			reference = 'A' - 10;
		} else {
			break;
		}
		digit = *str - reference;

		// Range check
		if (digit >= base) {
			break;
		}

		// Try to stuff this new digit onto the LSB end of the number
		if ((rval > cusp) || ((rval == cusp) && (digit > limit))) {
			overflow = 1;
			break;
		} else {
			rval = rval * base + digit;
		}
	}

	// FWIW, this is basically a radix check
	if (str == significand) {
		goto InvalidNumber;
	}

	// Handle negatives (part 2) and overflows
	// NOTE: Don't negate if we overflow because the standard specifies
	//       a special return value for that case
	// NOTE: When casting the output to signed long, the return value
	//       might technically be out of range (e.g. -4,000,000,000 is
	//       acceptable here).  Better to use the strtol() function in
	//       that case (once it's implemented).
	if (overflow) {
		rval = -1LL;
		errno = ERANGE;
	} else if (negative) {
		rval = -rval;
	}

Exit:

	// Handle the end of the numeric portion
	if (endptr) {
		*endptr = (char *)str;
	}

	return rval;

InvalidNumber:

	errno = EINVAL;
	str = nptr;
	rval = 0;
	goto Exit;
}

char *strrchr(const char *s, int c) {
	int ii;
	size_t l = strlen(s);
	
	if(!l) {
		return NULL;
	}
	
	for(ii = (int)(l-1); ii >=0; ii--) {
		if(s[ii] == c) { return (char *)&s[ii]; }
	}
	
	return NULL;
}

void __strcpy_i(char * s1, const char * s2, size_t n, size_t *ret) {
	size_t ii = 0;
	char *os1 = s1;
	for(ii = 0; ii < n; ii++) {
		*s1 = *s2;
		if(!*s2) break;
		s1++, s2++;
	}
	
	if(ret) *ret = ii;
}

void __strcat_i(char * s1, const char * s2, size_t n, size_t *ret) {
	size_t ii = 0, len = strlen(s1);
    s1 += len;
	for(ii = len; ii < n; ii++) {
        if(ii == (n-1)) { *s1 = '\0'; break; }
		*s1 = *s2;
		if(*s2 == '\0') { break; }
		s1++, s2++;
	}
	
	if(ret) *ret = ii;
}

char *strncpy(char * s1, const char * s2, size_t n) {
	__strcpy_i(s1, s2, n, NULL);
	
	return s1;
}

size_t strlcpy(char *dst, const char *src, size_t size) {
	size_t ret = 0;
	
	__strcpy_i(dst, src, size, &ret);
	
	return ret;
}

size_t strlcat(char *dst, const char *src, size_t size) {
	size_t ret = 0;
	
	__strcat_i(dst, src, size, &ret);
	
	return ret;
}

char *strcpy(char * s1, const char * s2) {
	char *os1 = s1;
	while((*s1++ = *s2++) != 0);
	return os1;
}

char *strcat(char * s1, const char * s2) {
	char* s;
	
	for(s = s1; *s != '\0'; ++s)
		;
	for(;(*s = *s2) != '\0';++s, ++s2)
		;
	
	return s1;
}

char *strncat(char * s1, const char * s2, size_t n) {
	strncpy(s1 + strlen(s1), s2, n);
	return s1;
}

static size_t _strspnhelper(const char *s1, const char *s2, int non_matching_span) {
	size_t l = strlen(s2);
	size_t count = 0;
	
	while(*s1) {
	
		int ii;
		for(ii = 0; ii < l; ii++) {
			if((*s1 == s2[ii]) == non_matching_span) { return count; }
		}
		
		s1++;
		count++;
	}
	
	return count;
}

size_t strcspn(const char *s1, const char *s2) {
	return _strspnhelper(s1, s2, 1);
}

size_t strspn(const char *s1, const char *s2) {
	return _strspnhelper(s1, s2, 0);
}

int putchar(int c) {
	//TRACER();
	char s[2] = {c, 0};
//	AsciiPrint(s);
	printf(s);
	return c;
}

char *strpbrk(const char *s1, const char *s2) {
	size_t l = strlen(s2);
	
	while(*s1) {
	
		int ii;
		for(ii = 0; ii < l; ii++) {
			if(*s1 == s2[ii]) { return (char*)s1; }
		}
		
		s1++;
	}
	
	return NULL;
}

void *memchr(const void *s, int c, size_t n) {
	char *s1 = (char*)s;
	while(n) {
		if(*s1 == (unsigned char)c) {return s1;}
		s1++;
		n--;
	}
	return NULL;
}

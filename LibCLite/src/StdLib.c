#include "StdLib.h"
#include "String.h"
#include "Errno.h"
#include "Ctype.h"
#include "Limits.h"
/*
#include EFI_PROTOCOL_CONSUMER(AppleShellInterpreter)
#include EFI_PROTOCOL_CONSUMER(AppleIpodRTC)
#include EFI_PROTOCOL_CONSUMER(AppleIpodPrng)
#include <EfiPrintLib.h>
#include <DataLib/DataLib.h>
#include <RTCLib/RTCLib.h>
#include <MathLib/MathLib.h>
#include <AppleCompileTimeAssert.h>
*/
typedef void (*at_exit_func)(void);
/*
static APPLE_IPOD_PRNG *gPseudorandom = NULL;
static APPLE_SHELL_INTERPRETER *gShell = NULL;
static APPLE_IPOD_RTC *gRTC = NULL;
*/
static at_exit_func _at_exit = NULL;


int atexit(void (*fptr)(void)) {
	if(_at_exit) {
		errno = ENOMEM;
		return -1;
	}
	
	_at_exit = fptr;
	return 0;
}

void exit(int j) {

	if(_at_exit) {
		_at_exit();
	}
	
	while(1);
}

void abort(void) {
	exit(EXIT_FAILURE);
}

int system(const char *string) {
/*	
	// Execute a command via the shell.
	EFI_STATUS res = EFI_LOAD_ERROR;
	BOOLEAN executed = FALSE;
	
	if(!gShell) {
		gBS->LocateProtocol(&gAppleShellInterpreterProtocolGuid, NULL,
			(VOID**)&gShell);
		if(!gShell) {
			LogError("No shell interpreter loaded...");
			return -1;
		}
	}
	
	res = gShell->ExecuteCommand(gShell, (char *)string, &executed);

	if(!executed) {
		
		//* from the man page:
		//* A return value of 127 means the execution of the shell failed.
		
		return 127;
	}

	if (res == 127 || res == -1)
	{
		LogError("ExecuteCommand returned unexpected value %d", res);
	}

	return EFI_STATUS_TO_INT(res);
*/
}

char *getenv(const char *name) {
	//printf_string("%a(%a)", __FUNCTION__, name);
	// Todo: hook this up
	return NULL;
}

time_t time(time_t *tloc) {
	/*
	EFI_STATUS Status;
	RTC_TIME Now;
	UINT32 ElapsedSecond;

	if (!gRTC) {
		Status = gBS->LocateProtocol(&gAppleIpodRTCProtocolGuid, NULL, (VOID **)&gRTC);
		if (EFI_ERROR(Status)) {
			LogError("Can't locate RTC");
			return -1;
		}
	}

	Status = gRTC->getDate(gRTC, &Now);
	if (EFI_ERROR(Status))
	{
		LogError("Failed calling getDate: %r", Status);
		return -1;
	}
	Status = RTCLib_date_to_secs(&Now, &ElapsedSecond);
	if (EFI_ERROR(Status))
	{
		LogError("Failed calling RTCLib_date_to_secs: %r", Status);
		return -1;
	}

	if (tloc) {
		*tloc = ElapsedSecond;
	}

	return ElapsedSecond;
	*/
}

time_t mktime(struct tm *timeptr) {
	// TODO: Do actual calculation
	// <rdar://problem/11581394> Implement mktime() in LibCLite
	LogError("mktime() currently unsupported");
	return -1;
}

struct tm *localtime(const time_t *timer) {
	return gmtime(timer);
}

struct tm *gmtime(const time_t *timer) {
	/*
	EFI_STATUS Status;
	RTC_TIME PmuRtcTime;
	static struct tm LocalTimeTZ;

	if (!timer) {
		return NULL;
	}

	Status = RTCLib_secs_to_date(*timer, &PmuRtcTime);
	if (EFI_ERROR(Status)) {
		return NULL;
	}

	LocalTimeTZ.tm_sec = PmuRtcTime.seconds;
	LocalTimeTZ.tm_min = PmuRtcTime.minutes;
	LocalTimeTZ.tm_hour = PmuRtcTime.hour24;
	LocalTimeTZ.tm_mday = PmuRtcTime.dayOfMonth;
	LocalTimeTZ.tm_mon = PmuRtcTime.calendarMonth - 1;
	LocalTimeTZ.tm_year = PmuRtcTime.calendarYear - 1900;
	LocalTimeTZ.tm_wday = PmuRtcTime.dayOfWeek;
	LocalTimeTZ.tm_yday = PmuRtcTime.dayOfYear;
	LocalTimeTZ.tm_isdst = 0;
	LocalTimeTZ.tm_gmtoff = 0;
	LocalTimeTZ.tm_zone = "UTC";

	return &LocalTimeTZ;
	*/
}

int difftimespec(struct timespec tv2, struct timespec tv1, struct timespec *diff) {
	int sign = 1;
	if (tv2.tv_sec < tv1.tv_sec) {
		struct timespec tmp = tv2;
		tv2 = tv1;
		tv1 = tmp;
		
		sign = -1;
	}
	else if (tv2.tv_sec == tv1.tv_sec && tv2.tv_nsec < tv1.tv_nsec) {
		struct timespec tmp = tv2;
		tv2 = tv1;
		tv1 = tmp;
		
		sign = -1;
	}

	if (tv2.tv_nsec < tv1.tv_nsec) {
		diff->tv_nsec = 1000000000 - (tv1.tv_nsec - tv2.tv_nsec);
		diff->tv_sec = tv2.tv_sec - tv1.tv_sec - 1;
	}
	else {
		diff->tv_nsec = tv2.tv_nsec - tv1.tv_nsec;
		diff->tv_sec = tv2.tv_sec - tv1.tv_sec;
	}

	return sign;
}


size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *timptr) {
	/*
	STATIC CONST CHAR8 *Days[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "???" };
	STATIC CONST CHAR8 *Months[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December", "???" };
	UINTN Remaining, CharsPrinted, NumExpected;
	CONST CHAR8 *DayName, *MonthName;
	CHAR8 *Output;
	UINTN Hour;

	// Check arguments
	if (!s || !format || !timptr || maxsize == 0) {
		return 0;
	}

	// Initialize output
	DayName = Days[EFI_MIN(timptr->tm_wday, countof(Days) - 1)];
	MonthName = Months[EFI_MIN(timptr->tm_mon, countof(Months) - 1)];
	Remaining = maxsize;
	Output = s;

	// Process the format string until we run out of output space (and reserve 1 output byte for terminator)
	CharsPrinted = NumExpected = 0;
	for (; *format && (Remaining > 1); ++format) {
		if (*format != '%') {
			*Output = *format;
			CharsPrinted = NumExpected = 1;
		} else {
			switch (*++format) {
				case 0: // bad format string
					CharsPrinted = NumExpected = 0;
					break;
				case 'a': // is replaced by the locale's abbreviated weekday name.
					CharsPrinted = AsciiSPrint(Output, Remaining, "%.3a", DayName);
					NumExpected = 3;
					break;
				case 'A': // is replaced by the locale's full weekday name.
					CharsPrinted = AsciiSPrint(Output, Remaining, "%a", DayName);
					NumExpected = AsciiStrLen(DayName);
					break;
				case 'b': // is replaced by the locale's abbreviated month name.
				case 'h': // same as %b.
					CharsPrinted = AsciiSPrint(Output, Remaining, "%.3a", MonthName);
					NumExpected = 3;
					break;
				case 'B': // is replaced by the locale's full month name.
					CharsPrinted = AsciiSPrint(Output, Remaining, "%a", MonthName);
					NumExpected = AsciiStrLen(MonthName);
					break;
				case 'c': // is replaced by the locale's appropriate date and time representation.
					CharsPrinted = strftime(Output, Remaining, "%a %b %e %H:%M:%S %Y", timptr);
					NumExpected = 24;
					break;
				case 'C': // is replaced by the century number (the year divided by 100 and truncated to an integer) as a decimal number [00-99].
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", (timptr->tm_year + 1900) / 100);
					NumExpected = 2;
					break;
				case 'd': // is replaced by the day of the month as a decimal number [01,31].
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", timptr->tm_mday);
					NumExpected = 2;
					break;
				case 'D': // same as %m/%d/%y.
				case 'x': // is replaced by the locale's appropriate date representation.
					CharsPrinted = strftime(Output, Remaining, "%m/%d/%y", timptr);
					NumExpected = 8;
					break;
				case 'e': // is replaced by the day of the month as a decimal number [1,31]; a single digit is preceded by a space.
					CharsPrinted = AsciiSPrint(Output, Remaining, "%2d", timptr->tm_mday);
					NumExpected = 2;
					break;
				case 'H': // is replaced by the hour (24-hour clock) as a decimal number [00,23].
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", timptr->tm_hour);
					NumExpected = 2;
					break;
				case 'I': // is replaced by the hour (12-hour clock) as a decimal number [01,12].
					Hour = timptr->tm_hour % 12;
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", (Hour == 0) ? 12 : Hour);
					NumExpected = 2;
					break;
				case 'j': // is replaced by the day of the year as a decimal number [001,366].
					CharsPrinted = AsciiSPrint(Output, Remaining, "%03d", timptr->tm_yday + 1);
					NumExpected = 3;
					break;
				case 'm': // is replaced by the month as a decimal number [01,12].
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", timptr->tm_mon + 1);
					NumExpected = 2;
					break;
				case 'M': // is replaced by the minute as a decimal number [00,59].
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", timptr->tm_min);
					NumExpected = 2;
					break;
				case 'n': // is replaced by a newline character.
					*Output = '\n';
					CharsPrinted = NumExpected = 1;
					break;
				case 'p': // is replaced by the locale's equivalent of either a.m. or p.m.
					CharsPrinted = AsciiSPrint(Output, Remaining, (timptr->tm_hour >= 12) ? "PM" : "AM");
					NumExpected = 2;
					break;
				case 'r': // is replaced by the time in a.m. and p.m. notation; in the POSIX locale this is equivalent to %I:%M:%S %p.
					CharsPrinted = strftime(Output, Remaining, "%I:%M:%S %p", timptr);
					NumExpected = 11;
					break;
				case 'R': // is replaced by the time in 24 hour notation (%H:%M).
					CharsPrinted = strftime(Output, Remaining, "%H:%M", timptr);
					NumExpected = 5;
					break;
				case 'S': // is replaced by the second as a decimal number [00,61].
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", timptr->tm_sec);
					NumExpected = 2;
					break;
				case 't': // is replaced by a tab character.
					*Output = '\t';
					CharsPrinted = NumExpected = 1;
					break;
				case 'T': // is replaced by the time (%H:%M:%S).
				case 'X': // is replaced by the locale's appropriate time representation.
					CharsPrinted = strftime(Output, Remaining, "%H:%M:%S", timptr);
					NumExpected = 8;
					break;
				case 'u': // is replaced by the weekday as a decimal number [1,7], with 1 representing Monday.
					CharsPrinted = AsciiSPrint(Output, Remaining, "%d", (timptr->tm_wday == 0) ? 7 : timptr->tm_wday);
					NumExpected = 1;
					break;
				case 'U': // is replaced by the week number of the year (Sunday as the first day of the week) as a decimal number [00,53].
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", (timptr->tm_yday + 7 - timptr->tm_wday) / 7);
					NumExpected = 2;
					break;
				case 'w': // is replaced by the weekday as a decimal number [0,6], with 0 representing Sunday.
					CharsPrinted = AsciiSPrint(Output, Remaining, "%d", timptr->tm_wday);
					NumExpected = 1;
					break;
				case 'W': // is replaced by the week number of the year (Monday as the first day of the week) as a decimal number [00,53]. All days in a new year preceding the first Monday are considered to be in week 0.
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", (timptr->tm_yday + 7 - (timptr->tm_wday ? (timptr->tm_wday - 1) : 6)) / 7);
					NumExpected = 2;
					break;
				case 'y': // is replaced by the year without century as a decimal number [00,99].
					CharsPrinted = AsciiSPrint(Output, Remaining, "%02d", timptr->tm_year % 100);
					NumExpected = 2;
					break;
				case 'Y': // is replaced by the year with century as a decimal number.
					CharsPrinted = AsciiSPrint(Output, Remaining, "%d", timptr->tm_year + 1900);
					NumExpected = 4;
					break;
				case 'Z': // is replaced by the timezone name or abbreviation, or by no bytes if no timezone information exists.
					if (timptr->tm_zone) {
						CharsPrinted = AsciiSPrint(Output, Remaining, "%a", timptr->tm_zone);
						NumExpected = 3;
					}
					break;
				case 'V': // is replaced by the week number of the year (Monday as the first day of the week) as a decimal number [01,53]. If the week containing 1 January has four or more days in the new year, then it is considered week 1. Otherwise, it is the last week of the previous year, and the next week is week 1.
					// TODO: Just fall through for now
					// <rdar://problem/11591060> Implement %V in strftime() in LibCLite to confirm with OpenGroup.org spec
				case '%': // is replaced by %.
					// Intentially fall through
				default:
					*Output = *format;
					CharsPrinted = NumExpected = 1;
					break;
			}
		}
		Output += CharsPrinted;
		Remaining -= EFI_MIN(Remaining, CharsPrinted);
		if ((*format == 0) || (CharsPrinted != NumExpected)) {
			break;
		}
	}

	// Always include a terminator to be safe
	// NOTE: If a recursive call to strftime() fails in the switch() construct above, this cuts off the string at the % boundary.
	*Output = 0;

	// Return the number of characters written if the output buffer was large enough to hold everything
	return *format ? 0 : (maxsize - Remaining);
	*/
}

long strtol(const char *str, char **endptr, int base) {
	int sign = 1, overflow = 0, limdval;
	long value = 0, lim;
	char dig;
	const char *subject;
	unsigned long maxval = 0UL;

	if (endptr) {
		*endptr = (char *)str;
	}

	if (!str) {
		errno = EINVAL;
		return 0;
	}

	// Ignore white space at beginning
	while (*str && isspace(*str)) {
		str++;
	}

	if (*str == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	// As mentioned in stdlib.h, accept we accept prefixes for numeric
	// literals in octal ("0"), hex ("0x"), and binary ("0b")
	//
	// Since the prefix is optional, we need to validate the first digit in
	// the subject string to decide whether the prefix can be skipped
	//
	// NOTE: Short-circuit boolean expression evaluation saves us from
	//       indexing past the end of a null-terminated string
	if ((base == 0 || base == 16) &&
		((str[0] == '0') && (toupper(str[1]) == 'X') && isxdigit(str[2]))) {
			base = 16;
			str += 2;
	} 
	if ((base == 0 || base == 2) &&
		((str[0] == '0') && (toupper(str[1]) == 'B') && ((unsigned)str[2] - '0' <= 1))) {
			base = 2;
			str += 2;
	}  
	if (base == 0) {
		if (*str == '0') {
			base = 8;
		} else {
			base = 10;
		}
	}

	subject = str;

	// Compute unit and non-unit portions of the cutoff value
	// for positive/negative overflows check
	maxval = (sign == -1) ? -(unsigned long)LONG_MIN : LONG_MAX;
	limdval = maxval % (unsigned long)base;
	lim = maxval / (unsigned long)base;

	while ((dig = toupper(*str)) != '\0') {
		long dval = -1;
		if (dig >= '0' && dig <= '9') {
			dval = dig - '0';
		} else if (dig >= 'A' && dig <= 'Z') {
			dval = dig - 'A' + 10;
		}

		if ((dval >= base) || (dval < 0)) {
			printf_string("Digit %c is invalid in base %d", *str, base);
			break;
		}

		// Check if an overflow condition has occured
		if (overflow || value > lim || (value == lim && dval > limdval)) {
			overflow = 1;
		} else {
			value = (value * base) + dval;
		}
        
		str++;
	}

	if (overflow) {
		value = (sign == 1) ? LONG_MAX : -LONG_MIN;
		errno = ERANGE;
	}

	// On the following conditions
	// - caller passed endptr
	// - not null string
	// - entire string is valid
	if (endptr && (str > subject)) {
		*endptr = (char *)str;
	}
	
	return sign * value;
}

long atol(const char *str) {
    return strtol(str, (char **)NULL, 10);
}

double strtod(const char *nptr, char **endptr) {
	double rval = 0, magnitude;
	const char *str = nptr, *significand;
	int negative = 0, digits = 0;
	int old_errno;
	
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
    
	// Convert
	significand = str;
	if (strnicmp(str, "inf", 3) == 0) {
		str += 3;
		rval = INFINITY;
		if (strnicmp(str, "inity", 5) == 0) {
			str += 5;
		}
	} else if (strnicmp(str, "nan", 3) == 0) {
		str += 3;
		rval = NAN;
	} else {
		// NOTE: We don't support hex floats (for now)
		// Build up the integer part
		for (; *str; ++str) {
			if (isdigit(*str)) {
				// NOTE: This will saturate to infinity automatically
				rval = rval * 10 + (*str - '0');
				++digits;
			} else {
				break;
			}
		}
		// Build up the fractional part
		// NOTE: Implement this as discrete step to avoid calling pow()
		//       for "simple" numbers
		if (*str == '.') {
			for (++str, magnitude = 10; *str; ++str, magnitude *= 10) {
				if (isdigit(*str)) {
					// NOTE: This will automatically throw out very small numbers
					rval += (*str - '0') / magnitude;
					++digits;
				} else {
					break;
				}
			}
		}
		// Safeguard against accepting a lone decimal point as input
		if (digits < 1) {
			goto InvalidNumber;
		}
		// Handle the exponent
		if ((*str == 'e') || (*str == 'E')) {
			old_errno = errno;
			errno = 0;
			magnitude = (long)strtoul(++str, (char **)&str, 10);
			if (errno != 0) {
				goto InvalidNumber;
			}
			errno = old_errno;
			rval *= pow(10, magnitude);
		}
	}
    
	// Update errno per the spec
	if (str == significand)
	{
		goto InvalidNumber;
	}
    
	// Handle negatives (part 2)
	if (negative) {
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

int rand(void) {
	/*
	EFI_STATUS Status;
	int Value;

	COMPILE_TIME_ASSERT(sizeof(Value) % sizeof(UINT32) == 0);

	if (gPseudorandom == NULL) {
		Status = gBS->LocateProtocol(&gAppleIpodPrngProtocolGuid, NULL, (VOID**)&gPseudorandom);
		REQUIRE_NO_ERROR(Status, Exit);
		Status = gPseudorandom->Seed(gPseudorandom, 1);
		REQUIRE_NO_ERROR(Status, Exit);
	}

	Status = gPseudorandom->GetRandomData(gPseudorandom, (UINT32 *)&Value, sizeof(Value));
	REQUIRE_NO_ERROR(Status, Exit);

	return (Value & RAND_MAX);

Exit:

	errno = ENODEV;
	*/
	return 0;
}

void srand(unsigned int seed) {
	/*
	EFI_STATUS Status;

	if (gPseudorandom == NULL) {
		Status = gBS->LocateProtocol(&gAppleIpodPrngProtocolGuid, NULL, (VOID**)&gPseudorandom);
		if (EFI_ERROR(Status)) {
			errno = ENODEV;
			return;
		}
	}

	gPseudorandom->Seed(gPseudorandom, seed);
	*/
}

void * __stack_chk_guard = NULL;
 
void __stack_chk_guard_setup()
{
    UINT32 * p;
    p = (UINT32 *) &__stack_chk_guard;
 
    *p =  0x00000aff;
}
 
void __stack_chk_fail()
{ 
	print_string("Stack was smashed!");
    EFI_DEADLOOP();
}


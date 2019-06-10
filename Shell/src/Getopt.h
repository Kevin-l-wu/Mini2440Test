#ifndef _GETOPT_H_
#define _GETOPT_H_

/*
 * GNU-like getopt_long() and 4.4BSD getsubopt()/optreset extensions
 */
#define no_argument        0
#define required_argument  1
#define optional_argument  2

struct option {
	/* name of long option */
	const char *name;
	/*
	 * one of no_argument, required_argument, and optional_argument:
	 * whether option takes an argument
	 */
	int has_arg;
	/* if not NULL, set *flag to val when option found */
	int *flag;
	/* if flag not NULL, value to set *flag to; else return value */
	int val;
};

int getopt_long(int, char * const *, const char *,
	    const struct option *, int *);
int	getopt_long_only(int, char * const *, const char *,
	    const struct option *, int *);

int	getopt( int, char * const *, const char *);

extern   char *optarg;                  /* getopt(3) external variables */
extern   int opterr;
extern   int optind;
extern   int optopt;
extern   int optreset;
 
#endif /* !_GETOPT_H_ */

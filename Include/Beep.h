#ifndef BEEP_H_
#define BEEP_H_

/*Led relevant GPIO define*/
#define GPBCON (*(volatile unsigned long*)0x56000010)
#define GPBDAT (*(volatile unsigned long*)0x56000014)

#endif
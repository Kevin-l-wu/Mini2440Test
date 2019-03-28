#ifndef _GPIO_H_
#define _GPIO_H_

//GPIO PORT B
#define GPBCON (*(volatile unsigned long*)0x56000010)
#define GPBDAT (*(volatile unsigned long*)0x56000014)


//GPIO PORT G
#define GPGCON (*(volatile unsigned int *)0x56000060)
#define GPGDAT (*(volatile unsigned int *)0x56000064)


#endif
#ifndef TIMER_H_
#define TIMER_H_


#define TCFG0            (*(volatile unsigned int *)0x51000000)
#define TCFG1            (*(volatile unsigned int *)0x51000004)
#define TCON             (*(volatile unsigned int *)0x51000008)

#define TCNTB0           (*(volatile unsigned int *)0x5100000C)
#define TCMPB0           (*(volatile unsigned int *)0x51000010)
#define TCNTO0           (*(volatile unsigned int *)0x51000014)

#define TCNTB1           (*(volatile unsigned int *)0x51000018)
#define TCMPB1           (*(volatile unsigned int *)0x5100001C)
#define TCNTO1           (*(volatile unsigned int *)0x51000020)

#define TCNTB2           (*(volatile unsigned int *)0x51000024)
#define TCMPB2           (*(volatile unsigned int *)0x51000028)
#define TCNTO2           (*(volatile unsigned int *)0x5100002C)

#define TCNTB3           (*(volatile unsigned int *)0x51000030)
#define TCMPB3           (*(volatile unsigned int *)0x51000034)
#define TCNTO3           (*(volatile unsigned int *)0x51000038)

#define TCNTB4           (*(volatile unsigned int *)0x5100003C)
#define TCNTO4           (*(volatile unsigned int *)0x51000040)


typedef enum _PRESCALER
{
	PRESCALER0 = 0,
	PRESCALER1 = 1,
} PRESCALER;

typedef enum _TIMER
{
	TIMER0 = 0,
	TIMER1 = 1,
	TIMER2 = 2,
	TIMER3 = 3,
	TIMER4 = 4,
	TIMER_NONE = 5,
} TIMER;

typedef enum _DMA_TIMER
{
	DMA_TIMER_NONE = 0x00,
	DMA_TIMER0 = 0x01,
	DMA_TIMER1 = 0x02,
	DMA_TIMER2 = 0x03,
	DMA_TIMER3 = 0x04,
	DMA_TIMER4 = 0x05,
} DMA_TIMER;


void Timer0Interrupt();
/*
MINI2440_STATUS TimerTest(int argc, char(*argv)[MAX_COMMAND_LENGTH]);
*/
#endif
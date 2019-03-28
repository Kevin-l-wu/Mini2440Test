#ifndef UART_H_
#define UART_H_

/*Uart relevant register define*/

#define GPHCON		(*(volatile unsigned int*)0x56000070)
#define CLKCON		(*(volatile unsigned int*)0x4C00000C)

#define ULCON0		(*(volatile unsigned int*)0x50000000)
#define UCON0		(*(volatile unsigned int*)0x50000004)
#define UFCON0		(*(volatile unsigned int*)0x50000008)
#define UMCON0		(*(volatile unsigned int*)0x5000000C)
#define UTRSTAT0	(*(volatile unsigned int*)0x50000010)
#define UERSTAT0	(*(volatile unsigned int*)0x50000014)
#define UFSTAT0		(*(volatile unsigned int*)0x50000018)
#define UMSTAT0		(*(volatile unsigned int*)0x5000001C)
#define UTXH0		(*(volatile unsigned int*)0x50000020)
#define URXH0		(*(volatile unsigned int*)0x50000024)
#define UBRDIV0		(*(volatile unsigned int*)0x50000028)

#define ULCON1		(*(volatile unsigned int*)0x50004000)
#define UCON1		(*(volatile unsigned int*)0x50004004)
#define UFCON1		(*(volatile unsigned int*)0x50004008)
#define UMCON1		(*(volatile unsigned int*)0x5000400C)
#define UTRSTAT1	(*(volatile unsigned int*)0x50004010)
#define UERSTAT1	(*(volatile unsigned int*)0x50004014)
#define UFSTAT1		(*(volatile unsigned int*)0x50004018)
#define UMSTAT1		(*(volatile unsigned int*)0x5000401C)
#define UTXH1		(*(volatile unsigned int*)0x50004020)
#define URXH1		(*(volatile unsigned int*)0x50004024)
#define UBRDIV1		(*(volatile unsigned int*)0x50004028)

#define ULCON2		(*(volatile unsigned int*)0x50008000)
#define UCON2		(*(volatile unsigned int*)0x50008004)
#define UFCON2		(*(volatile unsigned int*)0x50008008)
#define UMCON2		(*(volatile unsigned int*)0x5000800C)
#define UTRSTAT2	(*(volatile unsigned int*)0x50008010)
#define UERSTAT2	(*(volatile unsigned int*)0x50008014)
#define UFSTAT2		(*(volatile unsigned int*)0x50008018)
#define UMSTAT2		(*(volatile unsigned int*)0x5000801C)
#define UTXH2		(*(volatile unsigned int*)0x50008020)
#define URXH2		(*(volatile unsigned int*)0x50008024)
#define UBRDIV2		(*(volatile unsigned int*)0x50008028)


/* ULCONn Set Value */

#define NORMAL_MODE (0x00 << 6)
#define INFRARED_MODE (0x01 << 6)

#define PARITY_MODE_NONE (0b000 << 3)
#define PARITY_MODE_ODD (0b100 << 3)
#define PARITY_MODE_EVEN (0b101 << 3)
#define PARITY_MODE_FORCED1 (0b110 << 3)
#define PARITY_MODE_FORCED0 (0b111 << 3)

#define STOP_BIT1 (0x00 << 2)
#define STOP_BIT2 (0x01 << 2)

#define WORD_LENGTH5 (0b00 << 0)
#define WORD_LENGTH6 (0b01 << 0)
#define WORD_LENGTH7 (0b10 << 0)
#define WORD_LENGTH8 (0b11 << 0)


/* UCON Set Value */

#define FCLK_DIVIDER(value) (value << 12) 

#define CLK_SEL_PCLK (0b00 << 10)
#define CLK_SEL_UEXTCLK (0b00 << 10)
#define CLK_SEL_FCLK_N (0b00 << 10)

#define TX_INT_TYPE_PULSE (0 << 9)
#define TX_INT_TYPE_LEVEL (1 << 9)

#define RX_INT_TYPE_PULSE (0 << 8)
#define RX_INT_TYPE_LEVEL (1 << 8)

#define RX_TIMEOUT_DISABLE (0 << 7)
#define RX_TIMEOUT_ENABLE (1 << 7)

#define RX_ERR_STAT_INT_DISABLE (0 << 6)
#define RX_ERR_STAT_INT_ENABLE (1 << 6)

#define LOOPBACK_MODE_DISABLE (0 << 5)
#define LOOPBACK_MODE_ENABLE (1 << 5)

#define SEND_BREAK_SIG_DISABLE (0 << 4)
#define SEND_BREAK_SIG_ENABLE (1 << 4)

#define TX_MODE_DISABLE (0b00 << 2)
#define TX_MODE_INT (0b01 << 2)
#define TX_MODE_DMA03 (0b10 << 2)
#define TX_MODE_DMA1 (0b11 << 2)

#define RX_MODE_DISABLE (0b00 << 0)
#define RX_MODE_INT (0b01 << 0)
#define RX_MODE_DMA03 (0b10 << 0)
#define RX_MODE_DMA1 (0b11 << 0)

typedef enum _UART_INDEX
{
	UART0 = 0,
	UART1,
	UART2,
	UART_SUM,
} UART_INDEX;

typedef struct _UART_REGISTS
{
	UART_INDEX uart;
	volatile unsigned int* ulcon;
	volatile unsigned int* ucon;
	volatile unsigned int* ufcon;
	volatile unsigned int* umcon;
	volatile unsigned int* utrstat;
	volatile unsigned int* uerstat;
	volatile unsigned int* ufstat;
	volatile unsigned int* umstat;
	volatile unsigned int* utxh;
	volatile unsigned int* urxh;
	volatile unsigned int* ubrdiv;
} UART_REGISTS;


typedef enum {
	PrintOutTypeDec,
	PrintOutTypeHex,
} PRINT_OUT_TYPE;

typedef struct _PRINT_OUT
{
	PRINT_OUT_TYPE type;
	int bit_divesor;
	char* convert_table;
} PRINT_OUT;


void print_unsinged_data(unsigned unsigned_data, PRINT_OUT_TYPE print_out_type);
void print_int_data(int int_data, PRINT_OUT_TYPE print_out_type);

void print_string(const char* string);
void printf_string(const char* string, ...);

#endif

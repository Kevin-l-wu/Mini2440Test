#include <stdio.h>
#include <stdarg.h>
#include "GlobalDefine.h"
#include "Common.h"
#include "Uart.h"

#define BRDIV_115200 0x1a

static char deci_convert_table[10] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

static char hex_convert_table[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static UART_REGISTS gTimerRigisters[] = {
	{
		UART0,
		&ULCON0, &UCON0, &UFCON0, &UMCON0, &UTRSTAT0, &UERSTAT0, &UFSTAT0, &UMSTAT0, &UTXH0, &URXH0, &UBRDIV0,
	},
	{
		UART1,
		&ULCON1, &UCON1, &UFCON1, &UMCON1, &UTRSTAT1, &UERSTAT1, &UFSTAT1, &UMSTAT1, &UTXH1, &URXH1, &UBRDIV1,
	},
	{
		UART2,
		&ULCON2, &UCON2, &UFCON2, &UMCON2, &UTRSTAT2, &UERSTAT2, &UFSTAT2, &UMSTAT2, &UTXH2, &URXH2, &UBRDIV2,
	},
};

/* Find Out the uart No. */
static int FindOutUart(UART_INDEX uart)
{
	int ret = -1;
	int uartIndex = 0;
	
	for(uartIndex = 0; uartIndex < UART_SUM; uartIndex++)
	{
		if(gTimerRigisters[uartIndex].uart == uart)
		{
			ret = uartIndex;
			break;
		}
	}
	
	return ret;
}

/* Set ulcon register */
static void SetUlconReg(UART_INDEX uart, unsigned int val)
{
	int uartIndex = 0;
	
	uartIndex = FindOutUart(uart);
	if(uartIndex >= 0)
	{
		*(gTimerRigisters[uartIndex].ulcon) = val;
	}
}

/* Set ucon register */
static void SetUconReg(UART_INDEX uart, unsigned int val)
{
	int uartIndex = 0;
	
	uartIndex = FindOutUart(uart);
	if(uartIndex >= 0)
	{
		*(gTimerRigisters[uartIndex].ucon) = val;
	}
}

/* Set baud rate */
static void SetUbrdivReg(UART_INDEX uart, unsigned int val)
{
	int uartIndex = 0;
	
	uartIndex = FindOutUart(uart);
	if(uartIndex >= 0)
	{
		*(gTimerRigisters[uartIndex].ubrdiv) = val;
	}
}

int Uart0Init(UART_INDEX uart)
{
	/* Set GPIO function */
	GPHCON &= ~(0xf << 4);
    GPHCON |= (0xa << 4);

	SetUlconReg(UART0, STOP_BIT1 | WORD_LENGTH8);
	
	SetUconReg(UART0, TX_MODE_INT | RX_MODE_INT);
	
	SetUbrdivReg(UART0, BRDIV_115200);
}

int Uart0Send(unsigned char val)
{
	while (!(*(gTimerRigisters[0].utrstat) & (1 << 1)));
	*(gTimerRigisters[0].utxh) = val;
}

int Uart0Recv(unsigned char* val)
{
	while (!(*(gTimerRigisters[0].utrstat) & (1 << 0)));
	*val = *(gTimerRigisters[0].urxh);
}

void uart0_init()
{
    //Set GPIO function
	GPHCON &= ~(0xf << 4);
    GPHCON |= (0xa << 4);
	
	//Set data format
	ULCON0 = 0x03;

	//Set work pattern
	UCON0 = 0x05;
	
	//Set board rate
	UBRDIV0 = 0x1a; //115200
//	UBRDIV0 = 0x148;
//	UBRDIV0 = 0xA4; //19200
}

void uart1_init()
{
    //Set GPIO function
	GPHCON &= ~(0xf << 9);
    GPHCON |= (0xa << 9);
	
	//Set data format
	ULCON1 = 0x03;

	//Set work pattern
	UCON1 = 0x05;
	
	//Set board rate
	UBRDIV1 = 0x1a;
}

void print_string(const char* string)
{
	const char* string_temp = string;
	if(string_temp != NULL)
	{
		while(*string_temp)
		{
			Uart0Send(*string_temp);
			string_temp++;
		}
	}
}

void uart0_recv_string(char* recv_string)
{
	int index = 0;
	//For  Windows the end flag is "\r\n"
	char endFlagCR = 0;
	char endFlagNL = 0;
	
	if(recv_string != NULL)
	{
		for(index = 0; index < MAX_COMMAND_LENGTH * MAX_COMMAND_NUMBER; index++)
		{
			Uart0Recv(&recv_string[index]);
			
			if((recv_string[index] == '\r'))
			{
				endFlagNL = 1;
				recv_string[index] = '\0';
			}
			
			if((recv_string[index] == '\n'))
			{
				endFlagCR = 1;
				recv_string[index] = '\0';		
			}
			
			if(endFlagCR && endFlagCR)
			{
				endFlagNL = 0;
				endFlagCR = 0;
				break;
			}
		}
	}
}

void print_int_deci_data(int int_data)
{
	char bit_values[10] = {0};
	char bit_value = 0;
	int bit_divesor = 10;
	int index = 9;
	int start_bit = 10;
	
	bit_values[9] = int_data % 10 / 1;
	bit_values[8] = int_data % 100 / 10;
	bit_values[7] = int_data % 1000 / 100;
	bit_values[6] = int_data % 10000 / 1000;
	bit_values[5] = int_data % 100000 / 10000;
	bit_values[4] = int_data % 1000000 / 100000;
	bit_values[3] = int_data % 10000000 / 1000000;
	bit_values[2] = int_data % 100000000 / 10000000;
	bit_values[1] = int_data % 1000000000 / 100000000;
	bit_values[0] = int_data / 1000000000;
	
	for(index = 0; index < 10; index++)
	{
		if(bit_values[index] != 0)
		{
			start_bit = index;
			break;
		}
	}
	
	for(index = start_bit; index < 10; index++)
	{
		Uart0Send(bit_values[index] + '0');
	}
	
	if(start_bit == 10)
	{
		Uart0Send('0');
	}
}

void print_unsinged_data(unsigned unsigned_data, PRINT_OUT_TYPE print_out_type)
{
	char bit_values[10] = {0};
	int index = 0;
	int start_store_bit = 0;
	
	PRINT_OUT print_out = {0}; 
	
	if(print_out_type == PrintOutTypeDec)
	{
		print_out.type = PrintOutTypeDec;
		print_out.bit_divesor = 10;
		print_out.convert_table = deci_convert_table;
		start_store_bit = 9;									//Deci need 10 char buffer;
	}
	else
	{
		print_out.type = PrintOutTypeHex;
		print_out.bit_divesor = 16;
		print_out.convert_table = hex_convert_table;
		start_store_bit = 7;									//Hex need 8 char buffer, so start store bit 7;
	}
	
	bit_values[start_store_bit] = unsigned_data / caculate_power(print_out.bit_divesor, start_store_bit);
	
	for(index = start_store_bit - 1; index >= 0; index--)
	{

		bit_values[index] = (char)((unsigned_data % caculate_power(print_out.bit_divesor, index + 1))
			/ caculate_power(print_out.bit_divesor, index));
	}
	
	for(index = start_store_bit; index >= 0; index--)
	{	
		Uart0Send(print_out.convert_table[bit_values[index]]);
	}
}

void print_int_data(int int_data, PRINT_OUT_TYPE print_out_type)
{
	char bit_values[10] = {0};
	int index = 0;
	int start_store_bit = 0;
	int int_data_temp = 0;
	int is_negative_value = 0;
	int start_output_bit = 0;		//For flagging first None 0 bit
	
	PRINT_OUT print_out = {0}; 
	
	//if int_data < 0, set is_negative_value to 1, others, set to 0
	if(int_data < 0)
	{
		int_data_temp = (~int_data) + 1;
		is_negative_value = 1;
	}
	else
	{
		int_data_temp = int_data;
		is_negative_value = 0;
	}
	
	if(print_out_type == PrintOutTypeDec)
	{
		print_out.type = PrintOutTypeDec;
		print_out.bit_divesor = 10;
		print_out.convert_table = deci_convert_table;
		start_store_bit = 9;									//Deci need 10 char buffer;
	}
	else
	{
		print_out.type = PrintOutTypeHex;
		print_out.bit_divesor = 16;
		print_out.convert_table = hex_convert_table;
		start_store_bit = 7;									//Hex need 8 char buffer, so start store bit 7;
	}
	
	bit_values[start_store_bit] = int_data_temp / caculate_power(print_out.bit_divesor, start_store_bit);
	
	for(index = start_store_bit - 1; index >= 0; index--)
	{
		bit_values[index] = int_data_temp % caculate_power(print_out.bit_divesor, index + 1)
			/ caculate_power(print_out.bit_divesor, index);
	}

	for(index = start_store_bit; index >= 0; index--)
	{	
		if(bit_values[index] != 0)
		{
			start_output_bit = index;
			break;
		}
	}
	
	if(is_negative_value == 1)
	{
		Uart0Send('-');
	}
	
	for(index = start_output_bit; index >= 0; index--)
	{	
		Uart0Send(print_out.convert_table[bit_values[index]]);
	}
}

void print_char_to_hex(unsigned char char_data)
{
	int index = 0;
	unsigned char bit_values[2] = {0};
	
	bit_values[0] = char_data & 0x0f;
	bit_values[1] = (char_data >> 4) & 0x0f;

	for(index = 1; index >= 0; index--)
	{	
		Uart0Send(hex_convert_table[bit_values[index]]);
	}
}

void printf_string(const char* string, ...)
{
	const char* string_temp = string;
	va_list arg_ptr;
	va_start(arg_ptr, string);
	
	if(string_temp != NULL)
	{
		while(*string_temp)
		{
			if(*string_temp == '%')
			{
				switch(*(string_temp + 1))
				{
					int int_data;
					char char_data;
					unsigned char unsigned_char_data;
					char* string_data;
					unsigned unsigned_data;
					
					case 'd':
						int_data = va_arg(arg_ptr, int);
						print_int_data(int_data, PrintOutTypeDec);
						break;
						
					case 'c':
						char_data = (char)va_arg(arg_ptr, int);
						Uart0Send(char_data);
						break;
						
					case 'C':
						unsigned_char_data = (unsigned char)va_arg(arg_ptr, int);
						print_char_to_hex(unsigned_char_data);
						break;
						
					case 's':
						string_data = (char*)va_arg(arg_ptr, char*);
						print_string(string_data);
						break;
						
					case 'x':
						unsigned_data = (unsigned)va_arg(arg_ptr, int);
						print_unsinged_data(unsigned_data, PrintOutTypeHex);
						break;
						
					default:
						break;
				}
				
				string_temp++;
			}else
			{
				Uart0Send(*string_temp);
			}
					
			string_temp++;
		}
	}
	
	va_end(arg_ptr);
}
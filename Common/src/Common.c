#include "GlobalDefine.h"
#include "Stdio.h"
#include "String.h"
#include "Common.h"

void delay(int n)
{
	while(n--);
}

//The max result value should not > 2^31 - 1
int caculate_power(int value, int power)
{
	int ret = 1;
	int index = 0;
	
	for(index = 0; index < power; index++)
	{
		ret = ret * value;
	}

	return ret;
}

//0x12345678 to 0x78563412
int data_adjustment(unsigned* input)
{
	unsigned temp = 0;
	
	temp = temp | (*input & 0xff ) << 24;
	temp = temp | (*input & 0xff00) << 8;
	temp = temp | (*input & 0xff0000) >> 8;
	temp = temp | (*input & 0xff000000) >> 24;

	*input = temp;

	return 0;
}

void PrintAscii(unsigned char* buff, int length, int length_per_line)
{
	int i = 0;
	
	if(buff != 0 && length >= 0)
	{
		for(i = 0; i < length; i++)
		{
			if(((i % length_per_line) == 0))
			{
				printf("\n0x%x: ", (buff + i));
			}
			
			printf("0x%C ", buff[i]);
		}

		printf("\n");
	}
}

/***********************************************************************
Function Name:  		clean_string_prox
Input Paramater:		dest_string
Function Description:	Delete control char on the end of string
************************************************************************/
void clean_string_prox(char* dest_string)
{
	int str_index = 0;

	if(dest_string != NULL && dest_string[0] == '-' && dest_string[1] == '-')
	{
		while(dest_string[str_index + 2] != '\0')
		{
			dest_string[str_index] = dest_string[str_index + 1];
			dest_string[str_index + 1] = dest_string[str_index + 2];
			str_index++;
		}
	}
}

/***********************************************************************
Function Name:  		string_to_int
Input Paramater:		source_string
Function Description:	Convert string to int value
************************************************************************/
int string_to_int(char* source_string)
{
	int ret = 0;
	int str_index = 0;
	int string_length = strlen(source_string);
	int first_valid_bit = 0;			//If the last char not number, 

	//Flag first valid bit
	for(str_index = string_length - 1; str_index >= 0; str_index--)
	{
		if('0' <= source_string[str_index] && source_string[str_index] <= '9')
		{
			first_valid_bit = string_length - 1 - str_index;
			break;
		}
	}
	
	for(str_index = string_length - 1; str_index >= 0; str_index--)
	{
		if('0' <= source_string[str_index] && source_string[str_index] <= '9')
		{
			ret = ret + caculate_power(10, string_length - 1 - str_index - first_valid_bit) 
				* (source_string[str_index] - '0');
		}
	}
	
	return ret;
}

/***********************************************************************
Function Name:  		hex_string_to_int
Input Paramater:		source_string
Function Description:	Convert string to int value
************************************************************************/
int hex_string_to_int(char* source_string)
{
	int ret = 0;
	int str_index = 0;
	int string_length = strlen(source_string);
	int first_valid_bit = 0;			//If the last char not number, 
	

	//Flag first valid bit
	for(str_index = string_length - 1; str_index >= 0; str_index--)
	{
		if(('0' <= source_string[str_index] && source_string[str_index] <= '9') ||
			('a' <= source_string[str_index] && source_string[str_index] <= 'f') ||
			('A' <= source_string[str_index] && source_string[str_index] <= 'F'))
		{
			first_valid_bit = string_length - 1 - str_index;
			break;
		}
	}
	
	for(str_index = string_length - 1; str_index >= 0; str_index--)
	{
		if('0' <= source_string[str_index] && source_string[str_index] <= '9')
		{
			ret = ret + caculate_power(16, string_length - 1 - str_index - first_valid_bit) 
				* (source_string[str_index] - '0');
		}
		else if(('a' <= source_string[str_index] && source_string[str_index] <= 'f'))
		{
			ret = ret + caculate_power(16, string_length - 1 - str_index - first_valid_bit) 
				* (source_string[str_index] - 'a' + 10);
		}
		else if('A' <= source_string[str_index] && source_string[str_index] <= 'F')
		{
			ret = ret + caculate_power(16, string_length - 1 - str_index - first_valid_bit) 
				* (source_string[str_index] - 'A' + 10);
		}
	}
	
	return ret;
}

double pow( double x, double y )
{
	double result = 1;
	
	int i = 0;
	for(i = 0; i < y; i++)
	{
		result *= x;
	}

	return result;
}

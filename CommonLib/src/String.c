#include <stdio.h>
#include "String.h"

static char deci_convert_table[10] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

static char hex_convert_table[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

/***********************************************************************
Function Name:  		strcmp
Input Paramater:		dest_str1, dest_str2
Function Description:	Compare string dest_str1 and dest_str2, if 
						dest_str1 > dest_str2: 	return 1
						dest_str1 < dest_str2: 	return -1
						dest_str1 = dest_str2: 	return 0
						Invalid Paramater:		return -2
************************************************************************/
/*
int strcmp(char* dest_str1, char* dest_str2)
{
	int ret = 0;
	char* dest_str1_ptr = dest_str1;
	char* dest_str2_ptr = dest_str2;

	if(dest_str1 == NULL || dest_str2 == NULL)
	{
		return -2;
	}
		
	while(((*dest_str1_ptr) != '\0') && ((*dest_str2_ptr) != '\0'))
	{	
		if((*dest_str1_ptr) < (*dest_str2_ptr))
		{
			ret = -1;
			break;
		}
		if((*dest_str1_ptr) > (*dest_str2_ptr))
		{
			ret = 1;
			break;
		}
		dest_str1_ptr++;
		dest_str2_ptr++;
	}
	
	return ret;
}
*/

int strcmp(char* s1, char* s2)
{
	for( ; *s1 == *s2; ++s1, ++s2)
	{
		if(*s1 == '\0')
			return 0;
	}
	return (*(unsigned char*)s1 < *(unsigned char*)s2) ? -1 : 1;	
}

/***********************************************************************
Function Name:  		strlen
Input Paramater:		dest_string
Function Description:	Caculate dest_str length
************************************************************************/
int strlen(char* dest_str)
{
	int str_len = 0;

	if(dest_str != NULL)
	{
		while(dest_str[str_len] != '\0')
		{
			str_len++;
		}
	}
	
	return str_len;
}

/***********************************************************************
Function Name:  		strcpy
Input Paramater:		dest_str, src_str
Function Description:	Copy src_str to dest_str, return the number char copy
************************************************************************/
int strcpy(char* dest_str, char* src_str)
{
	int index = 0;
	
	if(dest_str != NULL && src_str != NULL)
	{
		while(src_str[index] != '\0')
		{
			dest_str[index] = src_str[index];
			index++;
		}
	}
	
	return index;
}


/***********************************************************************
Function Name:  		memcpy
Input Paramater:		dest_str, src_str
Function Description:	Copy src_str to dest_str, return the number char copy
************************************************************************/
int memcpy(char* dest_str, char* src_str, int length)
{
	int index = length;
	
	if(dest_str != NULL && src_str != NULL)
	{
		while(index)
		{
			dest_str[index - 1] = src_str[index - 1];
			index--;
		}
	}
	
	return length;
}

/***********************************************************************
Function Name:  		clean_string
Input Paramater:		dest_string
Function Description:	Delete control char on the end of string
************************************************************************/
void clean_string(char* dest_string)
{
	if(dest_string != NULL)
	{
		while(*dest_string)
		{
			if(*dest_string == '\count')
			{
				*dest_string = '\0';
				break;
			}
			
			dest_string++;
		}
	}
}

/***********************************************************************
Function Name:  		memset
Input Paramater:		dest_string, set_char, length
Function Description:	Use set_char to set the memory dest_string
************************************************************************/
void memset(void* dest_string, int set_char, unsigned length)
{
	unsigned index = 0;
	char* dest_string_temp = dest_string;
	
	if(dest_string != NULL)
	{
		for(index = 0; index < length; index++)
		{
			*dest_string_temp = set_char;
			dest_string_temp++;
		}
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

int memcmp(void *buf1, void *buf2, unsigned int count)
{
	
}

char *strcat(char *dest, const char *src)
{
	
}

char* strstr (char* str1, const char* str2)
{
	
}

char* strchr(char* s, int c)
{
	char ch = c;
	
	for( ; *s != ch; ++s)
	{
		if(*s == '\0')
		{
			return NULL;
		}
	}
	
	return s;
}

char* strrchr(char* str, int character)
{
	
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

char* strncpy(char* destination, const char* source, unsigned int num )
{
	
}

int strncmp(const char* str1, const char* str2, unsigned int num )
{
	
}


#include "GlobalDefine.h"
#include "Error.h"
#include "String.h"
#include "Common.h"

#include "Framework.h"

/*******************************************************************
 * Function Name: 	 	GenerateArgv
 * Inputs : 			argc, argv, receive_string
 * Outputs : 			void
 * Description:			Generate argc/argv
 * Author:				Kevin
 *******************************************************************/
void GenerateArgv(int* argc, char(*argv)[MAX_COMMAND_NUMBER][MAX_COMMAND_LENGTH], char* receive_string)
{
	char* receive_string_temp = receive_string;
	int str_length = strlen(receive_string) + 1;
	char* ptr_head = receive_string;
	int char_count = 0;
	int index = 0;
	
	*argc = 0;

	for(index = 0; index < str_length; index++)
	{
		char_count++;
		if(receive_string_temp[index] == ' ' || receive_string_temp[index] == '\0')
		{
			memcpy((*argv)[*argc], ptr_head, char_count);
			(*argv)[*argc][char_count - 1] = '\0';
			ptr_head = ptr_head + char_count;
			char_count = 0;
			*argc = *argc + 1;
			
			if(receive_string_temp[index] == '\0')
			{
				break;
			}
		}
	}
}

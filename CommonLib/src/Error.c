#include "Error.h"
#include "Uart.h"

void ErrorCheck(MINI2440_STATUS mini2440Status)
{
	switch(mini2440Status)
	{
		case MINI2440_SUCCESS:
			print_string("Success\n");
			break;
		case MINI2440_INVALID_PARAMETER:
			print_string("Invalid parameter\n");
			break;
		default:
			print_string("Other error\n");
			break;
	}
}

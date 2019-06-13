#include "Error.h"
#include "Stdio.h"

void ErrorCheck(MINI2440_STATUS mini2440Status)
{
	switch(mini2440Status)
	{
		case MINI2440_SUCCESS:
			printf("Success\n");
			break;
		case MINI2440_INVALID_PARAMETER:
			printf("Invalid parameter\n");
			break;
		default:
			printf("Other error\n");
			break;
	}
}

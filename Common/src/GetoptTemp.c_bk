#include "GlobalDefine.h"
#include "GetoptTemp.h"

int optInd;

void GetoptInit()
{
	optInd = 0;
}

char Getopt(int argc, char (*argv)[MAX_COMMAND_LENGTH], const char *shortopts)
{
	int index = 0;
	char ret = 0;
	
	for(index = optInd; index < argc; index++)
	{
		if(argv[index][0] == '-')
		{
			while(*shortopts)
			{
				if(argv[index][1] == *shortopts)
				{
					ret = *shortopts;
					optInd = index + 1;
					break;
				}
				
				shortopts++;
			}
			break;
		}
	}
	
	return ret;
}





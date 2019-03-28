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


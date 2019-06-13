#include "Stdio.h"
#include "Common.h"

void UndifinedInstruction()
{
	printf("UndifinedInstruction");
	delay(1024 * 2024);
	return ;
}

void SwiInterrupt()
{
	printf("SwiInterrupt\n");
	delay(1024 * 2024);
	return ;
}

void PrefetchAbort()
{
	printf("PrefetchAbort\n");
	delay(1024 * 2024);
	return ;
}

void DataAbort()
{
	printf("DataAbort\n");
	delay(1024 * 2024);
	return ;
}
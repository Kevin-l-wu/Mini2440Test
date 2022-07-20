#include "Stdio.h"
#include "Common.h"

void UndifinedInstruction()
{
	LogPrintf("UndifinedInstruction");
	delay(1024 * 2024);
	return ;
}

void SwiInterrupt()
{
	LogPrintf("SwiInterrupt\n");
	delay(1024 * 2024);
	return ;
}

void PrefetchAbort()
{
	LogPrintf("PrefetchAbort\n");
	delay(1024 * 2024);
	return ;
}

void DataAbort()
{
	LogPrintf("DataAbort\n");
	delay(1024 * 2024);
	return ;
}
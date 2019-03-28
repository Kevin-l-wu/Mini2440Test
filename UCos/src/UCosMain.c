#include <stdio.h>
#include "UCosMain.h"
#include "ucos_ii.h"

#include "ModManager.h"
#include "../Protocol/inc/TimerProtocol.h"

#define TASK_STK_SIZE 512
#define MSG_LEN 128


INT8U err;
INT32U OSTimes = 0;

char* ss = NULL;
char* s1 = NULL;
char* s2 = NULL;
char* s3 = NULL;
char* s4 = NULL;


void* msgGrp[MSG_LEN] = {0};

OS_EVENT* strQueue = NULL;
OS_STK taskStart[TASK_STK_SIZE] = {0};
OS_STK myTaskStk[TASK_STK_SIZE] = {0};
OS_STK yourTaskStk[TASK_STK_SIZE] = {0};


void DumpOSTCB(OS_TCB* pTCBCurr)
{
	int i = 0;
	print_string("------DumpOSTCB----\n");
	if(pTCBCurr != NULL)
	{
		printf_string("pTCBCurr->OSTCBPrio = 0x%x\n", pTCBCurr->OSTCBPrio);
		printf_string("pTCBCurr->OSTCBStkPtr = 0x%x\n", pTCBCurr->OSTCBStkPtr);
		
		for(i = 0; i < 16; i++)
		{
			printf_string("pTCBCurr->OSTCBStkPtr[%d] = 0x%x\n", i, pTCBCurr->OSTCBStkPtr[i]);
		}	
	}
}

void Func(void* pData)
{
	if(pData != NULL)
	{
		printf_string("%s call Func\n", (char*)pData);
	}
}

void MyTask(void* pData)
{
	#if OS_CRITICAL_METHOD == 3
		OS_CPU_SR cup_sr;
	#endif
	
	pData = pData;
	
//	print_string("-------MyTask()-------\n");
	
	for(;;)
	{	
		ss = OSQPend(strQueue, 0, &err);
		
		printf_string("%s\n", ss);
	
		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}

void YourTask(void* pData)
{
	#if OS_CRITICAL_METHOD == 3
		OS_CPU_SR cup_sr;
	#endif
	
	pData = pData;
	
//	print_string("-------YourTask()-------\n");
	
	for(;;)
	{
		ss = OSQPend(strQueue, 0, &err);
		
		printf_string("%s\n", ss);
	
		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}

void TaskStart(void* pData)
{
	#if OS_CRITICAL_METHOD == 3
		OS_CPU_SR cup_sr;
	#endif
	
	pData = pData;
	
//	print_string("-------TaskStart()-------\n");
	
	OSTaskCreate(MyTask, 0, &myTaskStk[TASK_STK_SIZE - 1], 6);
	OSTaskCreate(YourTask, 0, &yourTaskStk[TASK_STK_SIZE - 1], 7);
	
	s1 = "How many times this string can be received ?";
	OSQPostFront(strQueue, s1);
	
	for(;;)
	{
		OSTimes = OSTimeGet();
		
		printf_string("OSTimes = 0x%x\n", OSTimes);
		if( (10 < OSTimes) && (OSTimes < 50))
		{
			s2 = "The value of OSTimes is between 10 and 50";
			OSQPostFront(strQueue, s2);
			
			s3 = "Which task received this string?";
			OSQPostFront(strQueue, s3);
		}
		
		if( (100 < OSTimes) && (OSTimes < 150))
		{
			s4 = "The value of OSTimes is between 100 and 150";
			OSQPostFront(strQueue, s4);
		}
		
		print_string("TaskStart is running\n");
		OSTimeDly(1);
	}
}


static void StartTimer()
{
	TimerModOps* pTimerModOps = NULL;
	
	gLocateProtocol(MOD_TIMER, (void*)&pTimerModOps);
	
	if(pTimerModOps != NULL)	
	{
		pTimerModOps->TimerDelaySeconds(TIMER4, 1);
	}
}

void UCosMain()
{
	int i = 0;
	INT32U testPtr = 0;
	#if OS_CRITICAL_METHOD == 3                                  
    OS_CPU_SR  cpu_sr;
	#endif
	
	
	
	print_string("-------OSInit()-------\n");
	
	
	OSInit();
	
	strQueue = OSQCreate(&msgGrp[0], MSG_LEN);
	
	printf_string("strQueue->OSEventType = 0x%x\n", strQueue->OSEventType);
	
	OSTaskCreate(TaskStart, 0, &taskStart[TASK_STK_SIZE - 1], 5);
	
	print_string("-------OSStart()-------\n");
	
	StartTimer();
	
	OSStart();
}